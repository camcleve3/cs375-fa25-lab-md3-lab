#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8880

void handle_client(int client_sock) {
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_sock, buffer, sizeof(buffer));
        if (n <= 0 || strcmp(buffer, "exit") == 0) break;
        printf("[Ex3 C server] Received: %s\n", buffer);
        char reply[1060];
        snprintf(reply, sizeof(reply), "Echo: %s", buffer);
        write(client_sock, reply, strlen(reply));
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
