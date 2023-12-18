#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8069

int main() {
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    int serversocket;
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        perror("socket failed");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int bindstatus;
    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0) {
        perror("binding failed");
        return -1;
    } else {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    int clientsocket;
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0) {
        perror("connection is rejected by server");
        return -1;
    } else
        printf("connection is accepted\n");

    int i = 0, Tt, Tp, datasize;
    read(clientsocket, &Tt, sizeof(Tt));
    read(clientsocket, &Tp, sizeof(Tp));
    read(clientsocket, &datasize, sizeof(datasize));

    int data, timer = 0;
    while (i < datasize) {
        read(clientsocket, &data, sizeof(data));
        read(clientsocket, &timer, sizeof(timer));
        printf("Data packet with data %d received at %d\n", data, timer);

        timer = timer + Tt + Tp;
        write(clientsocket, &timer, sizeof(timer));

        i++;
    }

    close(serversocket);

    return 0;
}

