#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#define MAX 20
#define PORT 5000
#define SA struct sockaddr


bool isPrime(int n){
	int i, m;
	m = n/2;    
	for(i=2;i<=m;i++) {    
		if(n%i==0)    
		{    
			return false;    
		}    
	}
	return true;  
}

void verifyNumbers(int sockfd)
{
    char buff[MAX];
    char msgIsPrime[7] = "e primo";
    char msgIsNotPrime[11] = "nao e primo";
    int i;

    while (1) {
      bzero(buff, MAX);

      read(sockfd, buff, sizeof(buff));

      int n = atoi(buff);

      if (n == 0)
      {
        write(sockfd, buff, sizeof(buff));
        printf("Server Exit...\n");
        break;
      }
      
			bzero(buff, MAX);
			
      if(isPrime(n)){
    		for(i = 0; i < sizeof(msgIsPrime); i++)
    		{
    			buff[i] = msgIsPrime[i];
    		}
      }
      else{
    		for(i = 0; i < sizeof(msgIsNotPrime); i++)
    		{
    			buff[i] = msgIsNotPrime[i];
    		}
      }

      write(sockfd, buff, sizeof(buff));
    }
}

int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");
	
		
	verifyNumbers(connfd);

	close(sockfd);
}
