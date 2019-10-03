#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int a = 0;
	int pid = fork();
	if(pid == -1){
		printf("error");
	}
	else if(pid == 0){
		printf("ya rebionok\n");
	}
	else{
		sleep(1);
		printf("a ya roditel\n");
	}
	a++;
	//printf("%d %d %d\n", getpid(), getppid(), a);
	return 0;
}
