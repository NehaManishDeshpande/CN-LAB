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
	int sock, new_sock;
	struct sockaddr_in serv_addr, client_addr;
	socklen_t len;
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		printf("Creation failed...");
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	if((bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))<0)
	{
		printf("Connection failed...");
		return -1;
	}
	
	listen(sock, 5);
	len = sizeof(client_addr);
	
	if((new_sock = accept(sock, (struct sockaddr *)&client_addr, &len))<0)
	{
		printf("Connection failed...");
		return -1;
	}
	
	int Tp, Tt, datasize, timer = 0;
	
	
	read(new_sock, &Tp, sizeof(Tp));
	read(new_sock, &Tt, sizeof(Tt));
	read(new_sock, &datasize, sizeof(datasize));
	
	for(int i = 0; i < datasize; i++)
	{
		int data;	
		read(new_sock, &data, sizeof(data));
		read(new_sock, &timer, sizeof(timer));
		
		timer = timer + Tp + Tt;
		
        	printf("Data packet with data %d received at %d\n", data, timer);
		write(new_sock, &timer, sizeof(timer));
	}
	
	close(new_sock);
	close(sock);
	return 0;
}
