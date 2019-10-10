#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* ReadFifo(void* pFile){
	char* file =(char*) pFile;
	for(;;){
		char output[100];
		int fd = open(file, O_RDONLY);
		read(fd, output, 100);
		printf("%s", output);
		close(fd);
	}
}

void* WriteFifo(void* pFile){
	char* file =(char*) pFile;
	for(;;){
		char input[100];
		int fd = open(file, O_WRONLY);
		fgets(input, 100, stdin);
		write(fd, input, 100);
		close(fd);
	}
}


int main(int argc, char* argv[], char* envp[]){
	int first = atoi(argv[1]);
	pthread_t thid2, thid1;
	char file0[]="file0.fifo", file1[] = "file1.fifo";

	(void)umask(0);

	mknod(file0, S_IFIFO | 0666, 0);
	mknod(file1, S_IFIFO | 0666, 0);

	if (first == 0){
		pthread_create(&thid1, (pthread_attr_t *)NULL, WriteFifo, file0);
		pthread_create(&thid2, (pthread_attr_t *)NULL, ReadFifo, file1);
		pthread_join(thid1, (void **)NULL);
		pthread_join(thid2, (void **)NULL);
	}

	else if (first == 1){
		pthread_create(&thid1, (pthread_attr_t *)NULL, WriteFifo, file1);
		pthread_create(&thid2, (pthread_attr_t *)NULL, ReadFifo, file0);
		pthread_join(thid1, (void **)NULL);
		pthread_join(thid2, (void **)NULL);
	}

	return 0;

}
