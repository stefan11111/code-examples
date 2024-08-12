#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main()
{
    int file_fd = open("dst.txt", O_WRONLY | O_CREAT, 00664);
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
    if (connect(sock_fd, (const struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect");
        return 0;
    }
    char buf[4096];

    for (;;) {
        int nread = read(sock_fd, buf, sizeof(buf));
        if (nread == 0) {
            return 0;
        }

        if (nread < 0) {
            perror("read");
            return 0;
        }

        int nwrite = write(file_fd, buf, nread);
        if (nwrite < 0) {
            perror("write");
            return 0;
        }
    }
/* never reached */
    return 0;
}
