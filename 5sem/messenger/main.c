#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void ReadFifo(char* file){
	char output[100];
	int fd = open(file, O_RDONLY);
	read(fd, output, 100);
	printf("%s", output);
	close(fd);
}

void WriteFifo(char* file){
	char input[100];
	int fd = open(file, O_WRONLY);
	fgets(input, 100, stdin);
	write(fd, input, 100);
	close(fd);
}


int main(int argc, char* argv[], char* envp[]){
	int fd, first = atoi(argv[1]);
	char file0[]="file0.fifo", file1[] = "file1.fifo";

	(void)umask(0);

	mknod(file0, S_IFIFO | 0666, 0);
	mknod(file1, S_IFIFO | 0666, 0);

	if (first == 0){
		pid_t pid = fork();
		for(;;){
		if (pid > 0)
			WriteFifo(file0);
		else
			ReadFifo(file1);
		}
	}

	else if (first == 1){
		pid_t pid = fork();
		for(;;){
		if (pid > 0)
			WriteFifo(file1);
		else
			ReadFifo(file0);
		}
	}

	close(fd);
	return 0;

}





