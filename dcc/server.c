#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main()
{
    int file_fd = open("src.txt", O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        return 0;
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 0;
    }

    struct sockaddr_in address = (struct sockaddr_in){ AF_INET, htons(10000)};

    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) != 1) {
        fprintf(stderr, "inet_pton failed\n");
        return 0;
    }

    if (bind(sock_fd, (const struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        return 0;
    }

    if (listen(sock_fd, 100) < 0) {
        perror("listen");
        return 0;
    }

    int new_sock_fd = -1;

accept:

    new_sock_fd = accept(sock_fd, NULL, NULL);

    if (new_sock_fd < 0) {
        perror("accept");
        return 0;
    }

    struct stat statbuf;

    if (fstat(file_fd, &statbuf) < 0) {
        perror("fstat");
        return 0;
    }

    off_t size = statbuf.st_size;

    char buf[4096];

    for (;;) {
        int nread = read(file_fd, buf, sizeof(buf));
        if (nread < 0) {
            perror("read");
            return 0;
        }

        int nwrite = write(new_sock_fd, buf, nread);
        if (nwrite < 0) {
            perror("write");
            return 0;
        }

        size -= nwrite;

        if (size == 0) {
            shutdown(new_sock_fd, SHUT_RDWR);
            close(new_sock_fd);
            goto accept;
        }
    }
/* never reached */
    return 0;
}
