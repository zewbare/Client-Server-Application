#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 1000
#define PORTA 8080
#define PORTB 8081
#define SA struct sockaddr
void func(int sockfd)
{
	int flag=0;
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter Command : ");
		n = 0;
		// while ((buff[n++] = getchar()) != '\n');
		gets(buff);
		write(sockfd, buff, sizeof(buff));
		if ((strcmp(buff, "quit")) == 0) {
			flag=1;
		}
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("%s", buff);
		if(flag)
			break;
	}
}

int main(int argc, char* argv[])
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORTA);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	//load balancing
	char loadBalanceChecker[8];
	read(sockfd, loadBalanceChecker, 8);
	if(strcmp(loadBalanceChecker, "Occupied")==0){
		//change server
		close(sockfd);
		printf("ServerA occupied\n");

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			printf("socket creation failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		// assign IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(PORTB);
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!= 0) {
			printf("connection with the server failed...\n");
			exit(0);
		}
		else
			printf("connected to the server..\n");
	}

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);

}
