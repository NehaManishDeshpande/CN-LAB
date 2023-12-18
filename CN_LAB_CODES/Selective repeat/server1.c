#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_PACKETS 10 // Total number of packets to be sent by the client

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int recv_buffer[WINDOW_SIZE];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int expected_sequence = 0;
    int ack_packet[TOTAL_PACKETS]; // Acknowledge each packet separately

    while (expected_sequence < TOTAL_PACKETS) {
        int received = recv(new_socket, recv_buffer, sizeof(recv_buffer), 0);
        if (received <= 0) {
            printf("Connection closed or error occurred.\n");
            break;
        }

        printf("Received: ");
        int all_received = 1;
        for (int i = 0; i < WINDOW_SIZE; i++) {
            printf("%d ", recv_buffer[i]);
            if (recv_buffer[i] == expected_sequence) {
                ack_packet[recv_buffer[i]] = 1;
                expected_sequence++;
            } else {
                all_received = 0;
            }
        }
        printf("\n");

        if (all_received) {
            send(new_socket, ack_packet, sizeof(ack_packet), 0);
            printf("Acknowledgment sent.\n");
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

