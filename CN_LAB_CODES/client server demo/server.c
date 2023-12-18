#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int server_fd, new_socket, msg;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[1024];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    len = sizeof(client_addr);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&server_addr, &len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

	while(1)
	{
		bzero(buffer, 1024);
		msg = read(new_socket, buffer, 1024);
		if(msg < 0)
		{
			printf("Error ...");
			exit(1);
		}
		
		printf("Client --> %s", buffer);
		bzero(buffer, 1024);
		fgets(buffer, 1024, stdin);
		msg = write(new_socket, buffer, 1024);
		
		if(msg < 0)
		{
			printf("Error ...");
			exit(1);
		}
		if(!strncmp("bye", buffer, 3)){break;}
	}
    		close(new_socket);
    		close(server_fd);
    return 0;
}
