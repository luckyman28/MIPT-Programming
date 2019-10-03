#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main() {
	int f = open("./test.txt", O_RDONLY, 0);
	char* str = (char*)malloc(sizeof(char) * 8);
	int nbytes = read(f, str, 7);
	while (nbytes != 0){
		printf("%s\n", str);
		memset(str, 0, 7);// обнуляем буффер
		nbytes = read(f, str, 7);
	}
	free(str);
	close(f);
}



