static void slot_process(state l, char *buf, size_t buf_len, size_t i) {
    const char *err_str;

    int sock_fd = dcc_sessions.sock_fds[i].fd;
    int file_fd = dcc_sessions.slots[i].file_fd;

    if (!(dcc_sessions.sock_fds[i].revents & POLLIN)) {
        return;
    }

    int n = read(sock_fd, buf, buf_len);
    if (n == -1) {
        err_str = "read";
        goto handle_err;
    }

    if (n == 0) { /* EOF */
        goto close_fd;
    }

    dcc_sessions.slots[i].bytes_read += n;
    if (write(file_fd, buf, n) < 0) {
        err_str = "write";
        goto handle_err;
    }

    if (!(dcc_sessions.sock_fds[i].revents & POLLOUT)) {
        refresh_line(l);
        return;
    }
    size_t file_size = dcc_sessions.slots[i].file_size;
    size_t bytes_read = dcc_sessions.slots[i].bytes_read;
    unsigned ack_is_64 = file_size > UINT_MAX;
    unsigned ack_shift = (1 - ack_is_64) * 32;
    unsigned long long ack = htonll(bytes_read << ack_shift);
    if (write(sock_fd, &ack, ack_is_64 ? 8 : 4) < 0) {
        err_str = "write";
        goto handle_err;
    }

    refresh_line(l);

    if (bytes_read == file_size) {
        goto close_fd;
    }
    return;
handle_err:
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return;
    }
    if (errno == ECONNRESET) {
        goto close_fd;
    }
    perror(err_str);
    dcc_sessions.slots[i].err_cnt++;
    if (dcc_sessions.slots[i].err_cnt > ERR_MAX) {
        goto close_fd;
    }
    return;
close_fd:
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
    close(file_fd);
    slot_clear(i);
}
