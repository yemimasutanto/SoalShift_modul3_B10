#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/ipc.h>
#define SISOP B10
#include <sys/shm.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 9000

int server_fd, socket_new, valread;
int *stock;
int shmid;

void *print_stock(void* argv) {
    while(1) {
        printf("stok saat ini sebanyak : %d\n", *stock);
        sleep(5);
    }
}

void *build_shared_memory(void *argv) {
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);
    *stock = 10;
}

int main(int argc, char const *argv[]) 
{
    /* THREAD */
    pthread_t tid1, tid2;
    pthread_create(&(tid2), NULL, build_shared_memory, NULL);
    
    struct sockaddr_in address;
    int opt = 1, addrlen =  sizeof(address);
    char *success = "server penjual mengirim: transaksi berhasil";
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    // Memaksa socket agar terpasang pada port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("SetSockOpt Error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Memaksa socket agar terpasang pada port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding Failed");
        exit(EXIT_FAILURE);
    }

    // Listen to the client
    if (listen(server_fd, 3) < 0) {
        perror("Listen Error");
        exit(EXIT_FAILURE);
    }
    pthread_create(&(tid1), NULL, print_stock, NULL);
    if ((socket_new = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept Error");
        exit(EXIT_FAILURE);
    }
    
    while(1){        
        char buffer[1024] = {0};
        valread = read(socket_new, buffer, 1024);
        if (!valread) break;
        
        if (strcmp(buffer, "tambah") == 0) {
            *stock += 1;
            //send(socket_new, success, strlen(success), 0);
        } 
    }
    
    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}