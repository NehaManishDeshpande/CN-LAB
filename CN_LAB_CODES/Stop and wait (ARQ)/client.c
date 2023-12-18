#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8069

int main() {
    struct sockaddr_in serv_addr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("socket creation failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return -1;
    }

    int Tt, Tp, datasize, timer = 0;
    printf("Enter Tt and Tp: ");
    scanf("%d %d", &Tt, &Tp);
    write(sock, &Tt, sizeof(Tt));
    write(sock, &Tp, sizeof(Tp));

    printf("Enter data size: ");
    scanf("%d", &datasize);
    write(sock, &datasize, sizeof(datasize));

    printf("Enter data: ");
    for (int i = 0; i < datasize; i++) {
        int data;
        scanf("%d", &data);
        write(sock, &data, sizeof(data));
        write(sock, &timer, sizeof(timer));
        read(sock, &timer, sizeof(timer));
        printf("Ack received at %d ms\n", timer);
    }

    close(sock);
    return 0;
}

