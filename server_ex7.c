#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8880
#define MAX_CLIENTS 10

int client_socks[MAX_CLIENTS];
int client_count = 0;

void broadcast(const char* msg, int sender_sock) {
    for (int i = 0; i < client_count; ++i) {
        if (client_socks[i] != sender_sock) {
            write(client_socks[i], msg, strlen(msg));
        }
    }
}

void handle_client(int client_sock) {
    char buffer[1024];
    while (1) {
        int n = read(client_sock, buffer, sizeof(buffer));
        if (n <= 0 || strcmp(buffer, "exit") == 0) break;
        buffer[n] = 0;
        broadcast(buffer, client_sock);
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
    listen(server_sock, MAX_CLIENTS);

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_count < MAX_CLIENTS) {
            client_socks[client_count++] = client_sock;
            if (fork() == 0) {
                handle_client(client_sock);
                exit(0);
            }
        } else {
            close(client_sock);
        }
    }
    return 0;
}
