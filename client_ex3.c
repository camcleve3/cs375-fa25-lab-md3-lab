#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8880

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "exit") == 0) break;
        int n = read(sock, buffer, sizeof(buffer));
        buffer[n] = 0;
        printf("[Ex3 C client] Server response: %s\n", buffer);
    }
    close(sock);
    return 0;
}
