#include<stdio.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h> // read(), write(), close()
#define MAX 1000
// #define PORT 8080
#define SA struct sockaddr
int  serverCounter=0;
   
// Function designed for chat between client and server.
void serviceClient(int connfd)
{
    char command[MAX];
    int n;
    char* error= "Error!!";
    dup2(connfd, 1);
    // infinite loop for chat
    for (;;) {
        bzero(command, MAX);
        
        //printf("\n%d\n", connfd);
   
        // read the message from client and copy it in buffer
        if(n=read(connfd, command, sizeof(command))){
        // dup2(connfd, 0);
        // gets(buff);
            if(!strcasecmp(command, "quit")){
                serverCounter--;
                printf("Bye Client!");
                exit(0);
            }
            //dup2(connfd, 2);
            int s=system(command);
            // if(s==-1)
            //     printf("ERRORR!!\n");
        }
    }
}
   
// Driver function
int main(int argc, char* argv[])
{
    int sockfd, connfd, len, status, num=0;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    while(1){
        // printf("%d\n",servaddr.sin_port);
        // printf("%d\n", loadBalanceCounter);
        printf("%d\n", ++num);
        connfd = accept(sockfd, (SA*)&cli, &len);
    //     if(serverCounter>=5 && serverCounter<10 && servaddr.sin_port != htons(8081)){
    //         serverCounter++;
    //         //change to server B
    //         write(connfd, "Occupied", 8);
    //         close(connfd);
    //         continue;
    //     }
    //     else if(serverCounter>=10 && serverCounter%2==1 && servaddr.sin_port != htons(8081)){
    //         serverCounter++;
    //         //change to server B
    //         write(connfd, "Occupied", 8);
    //         close(connfd);
    //         continue;
    //     }
    //     else{
    //     serverCounter++;
    //     write(connfd, "Nocupied", 8);
    //     printf("Connected to client\n");
    //     if (connfd < 0) {
    //         printf("server accept failed...\n");
    //         exit(0);
    //     }
    //     else if(!fork())
    //         serviceClient(connfd);
    //     close(connfd);
    // }
    }
    close(sockfd);
}