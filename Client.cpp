// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



#include <thread>


void	connect_to_socket(const char *msg)
{
	int pid;

	pid = fork();
	if (!pid)
	{
		int 	sock = 0; 
		long 	valread;
		struct sockaddr_in serv_addr;
		static int PORT = 8000;

		char buffer[1024] = {0};
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("\n Socket creation error \n");
			// return -1;
		}
		
		printf("port = %i\n", PORT);

		memset(&serv_addr, '0', sizeof(serv_addr));
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);
		
		// Convert IPv4 and IPv6 addresses from text to binary form
		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
		{
			printf("\nInvalid address/ Address not supported \n");
			// return -1;
		}
		
		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("\n\t\t\t######### Connection Failed client side\n");
			// return -1;
		}
		
		send(sock , msg , strlen(msg) , 0 );
		printf("Hello message sent to port %i\n", PORT);
		valread = read( sock , buffer, 1024);
		printf("%s\n",buffer );
		exit(0);
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 2)
		return 0;
	connect_to_socket(argv[1]);
    return 0;
}