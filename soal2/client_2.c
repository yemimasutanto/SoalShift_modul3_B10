#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#define SISOP B10
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 9000

int main(int argc, char const *argv[])
{
    struct sockaddr_in address, serv_addr;
    int sock=0, valread;
    
    char *input = (char*)malloc(sizeof(char)*1024);
    
    // Creating Socket (var sock is socket descriptor)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket Creation Error\n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv6/IPv4 to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid Address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("can't connect to the server\n");
        return -1;
    }
    
    while(1) {
        char buffer[1024] = {0};
        int error = 0;
        socklen_t len = sizeof(error);
        int retVal = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);

        if (error != 0) {
            printf("server disconnected\n");
            break;
        }
        scanf("%s",input);
        send(sock, input, strlen(input), 0);
        valread = read(sock, buffer, 1024);
        printf("%s\n",buffer);
    }
    return 0;
}
