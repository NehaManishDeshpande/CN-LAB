#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server socket configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8082); // Use the same port as the server
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the server's IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send data to the server with byte count
        printf("Enter a message to send (or type 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);
        int byte_count = strlen(message);
        write(client_socket, &byte_count, sizeof(int));
        write(client_socket, message, strlen(message));



    // Close the client socket
    close(client_socket);

    return 0;
}

