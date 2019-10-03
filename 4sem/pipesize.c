#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
	int fd[2], result;
	pipe(fd);
	size_t size;
	int i = 0;
	while(1){
		printf("%d\n", i);
		++i;
		size = write(fd[1], "a", 1);
	}
}
