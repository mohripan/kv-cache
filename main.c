#include <asm-generic/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 6379

int main(void) {
    /* 1. Create the socket */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    /* 2. Let us restart the server without waiting */
    int yes = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt");
        return 1;
    }
    /* 3. Describe the address we want: 127.0.0.1:6379 */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    /* 4. Start listening */
    if (listen(server_fd, 16) < 0) {
        perror("listen");
        return 1;
    }
    printf("kv-cache listening on 127.0.0.1:%d\n", PORT);

    /* 5. Wait for one client */
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        return 1;
    }
    printf("client connected (fd %d)\n", client_fd);

    close(client_fd);
    close(server_fd);
    return 0;
}
