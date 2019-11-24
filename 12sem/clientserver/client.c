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


void* Read(void* Socket){
	int* sockfd =(int*) Socket;
	for(;;){
		char recvline[1000];
		recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *) NULL, NULL);
		printf("%s\n", recvline);
	}
}

void* Write(void* Socket){
	int* sockfd =(int*) Socket;

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);

	for(;;){
		char sendline[1000];
		fgets(sendline, 1000, stdin);
		sendto (sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
	}
}


int main(int argc, char* argv[], char* envp[]){
	pthread_t thid2, thid1;

	int sockfd;
	int n, len;
	char sendline[1000], recvline[1000];
	struct sockaddr_in servaddr, cliaddr;

	if(argc != 3){
		printf("Usage: a.out <IP address>\n");
		exit(1);
	}
	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror(NULL);
		exit(1);
	}

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;cliaddr.sin_port = htons(atoi(argv[2]));
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;

	servaddr.sin_port = htons(51000);

	if (inet_aton(argv[1], &servaddr.sin_addr) == 0){
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	(void)umask(0);

	pthread_create(&thid1, (pthread_attr_t *)NULL, Write, sockfd);
	pthread_create(&thid2, (pthread_attr_t *)NULL, Read, sockfd);
	pthread_join(thid1, (void **)NULL);
	pthread_join(thid2, (void **)NULL);

	return 0;

}
