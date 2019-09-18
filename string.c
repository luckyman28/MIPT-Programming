#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void GeneratingString(int n, char* string){
	char* string2 = string;
	for(int i = 1; i < n + 1; ++i){
		string[(1<<(i-1)) - 1] = 'a' + i - 1;
		strncpy(string + (1<<i) , string, ((1<<i)-1));
	}
	printf("%s", string);
}

int main(){
	int n;
	scanf("%d", &n);
	char* string = (char*) malloc(1000 * sizeof(char));
	GeneratingString(n, string);
	return 0;
}

