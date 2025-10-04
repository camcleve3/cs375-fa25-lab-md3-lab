#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8880

void handle_client(int client_sock) {
    char buffer[1024];
    fd_set readfds;
    struct timeval timeout;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(client_sock, &readfds);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        int ready = select(client_sock + 1, &readfds, NULL, NULL, &timeout);
        if (ready > 0) {
            int n = read(client_sock, buffer, sizeof(buffer));
            if (n <= 0 || strcmp(buffer, "exit") == 0) break;
            printf("[Ex5 C server] Received: %s\n", buffer);
            write(client_sock, "Echo", 4);
        } else {
            printf("[Ex5 C server] Timeout, closing connection.\n");
            break;
        }
    }
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (fork() == 0) {
            handle_client(client_sock);
            exit(0);
        }
    }
    return 0;
}
