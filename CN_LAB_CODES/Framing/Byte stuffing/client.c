#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8086

void byteStuff(char* input, char* output) {
    int inputLength = strlen(input);
    int outputIndex = 0;

    for (int i = 0; i < inputLength; i++) {
        output[outputIndex++] = input[i];

        if (input[i] == 0x40) {
            output[outputIndex++] = 0x23;  // Add 0x55 after encountering 0xAA
        }
    }

    output[outputIndex] = '\0';  // Null-terminate the output string
}

int main() {
    struct sockaddr_in serveraddress;
    int clientsocket;

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket < 0) {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if(connection_status < 0) {
        printf("connection failed\n");
        return -1;
    }
    else {
        printf("connection established\n");
    }

    char input[256];
    char output[512]; // Double the size to accommodate possible additional bytes due to stuffing

    printf("Enter Data: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline from fgets

    byteStuff(input, output);

    printf("After byte stuffing: %s\n", output);
   
    send(clientsocket, output, strlen(output), 0);

    close(clientsocket);

    return 0;
}

