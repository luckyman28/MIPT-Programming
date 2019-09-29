#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int n;
	scanf("%d", &n);
	for(int i=0; i < n; ++i){
		pid_t pid = fork();
		if (pid == 0){
			printf("moego papu zovyt %d\n", getppid());
			exit(57);
		}
	}
	sleep(1);
	return 0;
}
