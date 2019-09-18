#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void split(char* str, char* delim){
	char* first = strtok(str, delim);
	while (first != NULL){
		printf("%s\n", first);
		first = strtok(NULL, delim);
	}
}

int main(){
	char str[10000];
	fgets(str, 100, stdin);
	char delim[10000];
	fgets(delim, 100, stdin);
	split(str, delim);
	return 0;
}


