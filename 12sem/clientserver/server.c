#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

int port;

void* Read(void* Socket){
	int sockfd = *(int*)Socket;
	struct sockaddr_in cliaddr;
	for(;;){
		char recvline[1000];
		int clilen = sizeof(cliaddr);
		recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *) &cliaddr, &clilen);
		printf("%s\n", recvline);
	}
}

void* Write(void* Socket){
	int sockfd = *(int*) Socket;
	struct sockaddr_in cliaddr;
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(port);
	for(;;){
		char sendline[1000];
		int clilen = sizeof(cliaddr);
		fgets(sendline, 1000, stdin);
		sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &cliaddr, clilen);
	}
}

int main(int argc, char* argv[], char* envp[]){
	  pthread_t thid2, thid1;

	  int sockfd;
	  int clilen, n;
	  char line[1000];
	  struct sockaddr_in servaddr, cliaddr;
	  bzero(&servaddr, sizeof(servaddr));
	  servaddr.sin_family = AF_INET;
	  servaddr.sin_port = htons(51000);
	  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror(NULL);
		exit(1);
	  }
	  if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	  }

	  port = atoi(argv[2]);

	  bzero(&cliaddr, sizeof(cliaddr));
	  cliaddr.sin_family = AF_INET;
	  cliaddr.sin_port = htons(atoi(argv[2]));

	  if (inet_aton(argv[1], &cliaddr.sin_addr) == 0){
			printf("Invalid IP address\n");
			close(sockfd);
			exit(1);
	  }
	  (void)umask(0);

	  pthread_create(&thid1, (pthread_attr_t *)NULL, Read, &sockfd);
      pthread_create(&thid2, (pthread_attr_t *)NULL, Write, &sockfd);
	  pthread_join(thid1, (void **)NULL);
	  pthread_join(thid2, (void **)NULL);

	  return 0;

	}
