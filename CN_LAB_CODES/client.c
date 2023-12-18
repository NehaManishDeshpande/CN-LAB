#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8087

int main()
{
	int  sock;
	struct sockaddr_in serv_addr;
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		printf("Creation failed...");
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	if((connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))<0)
	{
		printf("Connection failed...");
		return -1;
	}
	
	int Tp, Tt, datasize, timer = 0;
	
	printf("Enter Tp and Tt: ");
	scanf("%d%d", &Tp, &Tt);
	printf("datasize: ");
	scanf("%d", &datasize);
	
	write(sock, &Tp, sizeof(Tp));
	write(sock, &Tt, sizeof(Tt));
	write(sock, &datasize, sizeof(datasize));
	
	printf("Enter data: ");
	for(int i = 0; i < datasize; i++)
	{
		int data;
		
		scanf("%d", &data);
		write(sock, &data, sizeof(data));
		write(sock, &timer, sizeof(timer));
		
		int n = read(sock, &timer, sizeof(timer));
		printf("ACK for data in time %d ms", timer);
	}
	
	close(sock);
	return 0;
}
