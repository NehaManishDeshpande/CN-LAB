#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8082

int main() {
    int server_fd, new_socket, msg;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[1024];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    len = sizeof(client_addr);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1) {
        bzero(buffer, 1024);
        msg = read(new_socket, buffer, 1024);
        if(msg < 0) {
            printf("Error ...\n");
            exit(1);
        }
        
        printf("Client --> %s", buffer);
	if(!strncmp("bye", buffer, 3)){break;}
	else
	{
		int num1, num2;
		char op;
		
		if(sscanf(buffer, "%d %c %d", &num1, &op, &num2) != 3)
		{
			printf("Invalid expression");
			strcpy(buffer, "Invalid expression");
		}
		else{
			int result;
			
			switch(op)
			{
				case '+':
					result = num1 + num2;
					sprintf(buffer, "Result : %d", result);
					break;
				case '-':
					result = num1 - num2;
					sprintf(buffer, "Result : %d", result);
					break;
				case '*':
					result = num1 * num2;
					sprintf(buffer, "Result : %d", result);
					break;
				case '/':
					if(num2 == 0)
					{
						printf("divide by zero error...");
						strcpy(buffer, "divide by zero error..");
					}
					else
					{
					result = num1 / num2;
					sprintf(buffer, "Result : %d", result);
					}
					break;
				default:
					printf("invalid operator");
					break;
			}
	}
       

            msg = write(new_socket, buffer, strlen(buffer));
            if(msg < 0) {
                printf("Error ...\n");
                exit(1);
            }
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

