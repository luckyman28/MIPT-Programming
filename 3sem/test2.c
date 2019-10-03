#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
	pid_t pid = fork();
	if (pid == 0){
		execlp("gcc", "gcc", "test1.c", "-o", "test1.out", NULL);
	}
	else{
		//int status;
		//wait(&status);
		pid_t wait(int* status);
		execlp("./test1.out", "./test1.out", NULL);
	}
	printf("Error\n");
	return 0;
}
