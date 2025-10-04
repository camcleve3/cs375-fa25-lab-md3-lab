#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8880

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = "Hello C++ Server";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    send(sock, buffer, strlen(buffer), 0);
    read(sock, buffer, sizeof(buffer));
    std::cout << "[C++ client] Server response: " << buffer << std::endl;
    return 0;
}
