#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = htonl(0);
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char msg[] = "hello server";
    ssize_t n = write(fd, msg, strlen(msg));
    if (n < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    char rbuf[64] = {};
    ssize_t m = read(fd, rbuf, sizeof(rbuf) - 1);
    if (m < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("recv from server: %s\n", rbuf);
    close(fd);
}