#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8082

int main() {
    int sock, msg;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    while(1) {
        printf("Enter an expression (e.g., 5 + 3) or 'bye' to exit: ");
        bzero(buffer, 1024);
        fgets(buffer, 1024, stdin);

        if (strncmp("bye", buffer, 3) == 0) {
            printf("Closing connection...\n");
            msg = write(sock, buffer, strlen(buffer));
            break;
        } else {
            msg = write(sock, buffer, strlen(buffer));
        }

        if(msg < 0) {
            printf("Error writing to socket...\n");
            exit(1);
        }
        
        bzero(buffer, 1024);
        msg = read(sock, buffer, 1024);
        if (msg < 0) {
            printf("Error reading from socket...\n");
            exit(1);
        }
        
        printf("Server response: %s\n", buffer);
    }

    close(sock);
    return 0;
}

