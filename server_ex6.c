#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

#define PORT 8880
#define LOGFILE "error_log.txt"

void log_error(const char* msg) {
    FILE* f = fopen(LOGFILE, "a");
    time_t now = time(NULL);
    fprintf(f, "%s: %s\n", ctime(&now), msg);
    fclose(f);
}

void handle_client(int client_sock) {
    char buffer[1024];
    int n = read(client_sock, buffer, sizeof(buffer));
    if (n < 0) log_error("read failed");
    write(client_sock, "Hello from server", 17);
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) log_error("socket failed");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) log_error("bind failed");
    if (listen(server_sock, 5) < 0) log_error("listen failed");

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) log_error("accept failed");
        if (fork() == 0) {
            handle_client(client_sock);
            exit(0);
        }
    }
    return 0;
}
