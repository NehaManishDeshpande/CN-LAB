#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8080

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
    
    while(1)
	{
		bzero(buffer, 1024);
		fgets(buffer, 1024, stdin);
		msg = write(sock, buffer, strlen(buffer));
		
		if(msg < 0)
		{
			printf("Error ...");
			exit(1);
		}
		
		bzero(buffer, 1024);
		msg = read(sock, buffer, 1024);
		
		printf("Server--> %s", buffer);
		if(!strncmp("bye", buffer, 3)){break;}
	}
    		close(sock);
    return 0;
}
