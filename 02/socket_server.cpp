#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void do_sth(int fd) {
    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        perror("read");
        return;
    }
    printf("recv from client: %s\n", rbuf);

    char wbuf[64] = "hello client";
    ssize_t m = write(fd, wbuf, strlen(wbuf));
    if (m < 0) {
        perror("write");
        return;
    }
}


int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr  {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen
    rv = listen(fd, 5);
    if (rv) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // accept connections
    while (true) {
        // accept
        struct sockaddr_in client_addr {};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        // handle client connection
        do_sth(client_fd);
        close(client_fd);
    }
}