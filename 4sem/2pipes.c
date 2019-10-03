#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
	int fd1[2], result;
	int fd2[2], result;
	pipe(fd1);
	pipe(fd2);
	pid_t pid = fork();
	if(pid > 0){
		int a, b, c;
		scanf("%d %d", &a, &b);
		size = write(fd1[1], &a, sizeof(int));
		size = write(fd1[1], &b, sizeof(int));
		size = read(fd2[0], &c, sizeof(int));
	}
	else(pid == 0){
		int a, b









