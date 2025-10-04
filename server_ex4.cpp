#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PORT 8880

void handle_client(int client_sock, int* client_count) {
    __sync_add_and_fetch(client_count, 1);
    char buffer[1024];
    read(client_sock, buffer, sizeof(buffer));
    std::cout << "[Ex4 C++ server] Received: " << buffer << std::endl;
    write(client_sock, "Hello from C++ server", 21);
    close(client_sock);
    __sync_sub_and_fetch(client_count, 1);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int* client_count = (int*)shmat(shmid, nullptr, 0);
    *client_count = 0;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    while (true) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (fork() == 0) {
            handle_client(client_sock, client_count);
            std::cout << "Active clients: " << *client_count << std::endl;
            exit(0);
        }
    }
    shmdt(client_count);
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}
