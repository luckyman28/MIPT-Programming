#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i){
		pid_t pid = fork();
		if (pid == 0){
			printf("u %d rodilsya rebionok nomer %d\n", getppid(), getpid());
		}
		else{
			pid_t wait(int* status);
			sleep(1);
                        printf("u %d zakonchilsya rebionok nomer %d , svetlaya pamiat\n", getpid(), pid);
                        sleep(1);
			exit(57);
		}
	}
	return 0;
}
