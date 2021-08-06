#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#define MAX 20
#define PORT 5000
#define SA struct sockaddr


void sendNumbers(int sockfd)
{
    char buff[MAX];
    int q = 0;
    int n = 1;
		bzero(buff, MAX);
    printf("Este programa gera numeros aleatorios em ordem crescente e verifica se estes sao numeros primos \n");
    printf("Digite a quantidade desejada de numeros para serem verificados: %s", buff);

    bzero(buff, MAX);
		while ((buff[q++] = getchar()) != '\n');
    q = atoi(buff);
    
    while (1) {
        bzero(buff, sizeof(buff));

        if(q > 0)
        {
          n += rand() % 100 + 1;
          snprintf(buff, sizeof(buff), "%d", n);
          printf("To Server : %s \n", buff);
		      write(sockfd, buff, sizeof(buff));
		      bzero(buff, sizeof(buff));
		      read(sockfd, buff, sizeof(buff));
		      printf("From Server : %s \n", buff);
        }
        else
        {
          snprintf(buff, sizeof(buff), "%d", 0);
		      write(sockfd, buff, sizeof(buff));
		      bzero(buff, sizeof(buff));
          printf("Client Exit...\n");
          break;
        }
        
        q--;
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    
    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    sendNumbers(sockfd);

    close(sockfd);
}
