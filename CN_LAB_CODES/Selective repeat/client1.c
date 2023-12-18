#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_PACKETS 10 // Total number of packets to send

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int sequence = 0;
    int ack_packet[TOTAL_PACKETS]; // Acknowledgment array to track received packets

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (sequence < TOTAL_PACKETS) {
        int data_to_send[WINDOW_SIZE];
        int packet_count = 0;

        for (int i = 0; i < WINDOW_SIZE; i++) {
            int current_sequence = sequence++;
            if (current_sequence < TOTAL_PACKETS) {
                data_to_send[i] = current_sequence;
                packet_count++;
            }
        }

        send(sock, data_to_send, sizeof(data_to_send), 0);
        printf("Sent: ");
        for (int i = 0; i < packet_count; i++) {
            printf("%d ", data_to_send[i]);
        }
        printf("\n");

        recv(sock, ack_packet, sizeof(ack_packet), 0);

        printf("Acknowledgment received.\n");

        // Check for missing packets and retransmit them
        for (int i = 0; i < TOTAL_PACKETS; i++) {
            if (ack_packet[i] == 0) {
                // Retransmit the missing packet
                int resend_data[1] = {i};
                send(sock, resend_data, sizeof(resend_data), 0);
                printf("Retransmitted packet: %d\n", i);
            }
        }
    }

    return 0;
}

