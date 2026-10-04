#include <asm-generic/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt");
        return 1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(6379);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(fd, 16) < 0) {
        perror("listen");
        return 1;
    }
    printf("listening on 127.0.0.1:6379\n");

    int client_fd = accept(fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        return 1;
    }
    printf("client connected, fd = %d\n", client_fd);

    char buf[1024];
    ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("read");
        return 1;
    }
    buf[n] = '\0';
    printf("got %zd bytes: %s", n, buf);

    close(client_fd);
    close(fd);

    return 0;
}
