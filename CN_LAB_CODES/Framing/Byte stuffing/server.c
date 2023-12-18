#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8086

void byteUnstuff(char *output, char *final) {
    int outputLength = strlen(output);
    int finalIndex = 0;
    int skipNextByte = 0;

    for (int i = 0; i < outputLength; i++) {
        if (skipNextByte) {
            skipNextByte = 0;
            continue;
        }

        if (output[i] == 0x40 && output[i + 1] == 0x23) {
            final[finalIndex++] = output[i];
            skipNextByte = 1;
        } else {
            final[finalIndex++] = output[i];
        }
    }

    final[finalIndex] = '\0';  // Null-terminate the final string
}

int main() {
    int serversocket, clientsocket, bindstatus;
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        printf("socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0) {
        printf("binding failed\n");
        return -1;
    }
    else {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("waiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0) {
        printf("connection is rejected by server\n");
        return -1;
    }
    else {
        printf("connection is accepted\n");
    }

    char output[512]; // Double the size to accommodate possible additional bytes due to stuffing
    char final[256];

    recv(clientsocket, output, sizeof(output), 0);
    printf("Data received after byte stuffing: %s\n", output);

    byteUnstuff(output, final);
    printf("Retrieved original data: %s\n", final);

    close(serversocket);
    return 0;
}

