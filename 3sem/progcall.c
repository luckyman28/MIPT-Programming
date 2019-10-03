#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct mystruct
{
    int waittime;
    char** token;
} Line;

void LineScan(int num, FILE *file, Line *line){
	for(int i = 0; i < num; i++)
    {
	line[i].token = (char**)malloc(100 * sizeof(char*));
	char* str = (char*)malloc(100 * sizeof(char));
	char* delim = " \n";
	fgets(str, 100, file);
	char* first = strtok(str, delim);
	line[i].waittime = atoi(first);
	first = strtok(str, delim);
	int k = 0;
    while (first != NULL){
    	line[i].token[k] = (char*)malloc(20 * sizeof(char));
        line[i].token[k] = first;
        first = strtok(NULL, delim);
        k++;
        }
    }
}

int main()
{
    int num;
    FILE *file = fopen("/home/vasily/tesalin/MIPT-Programming/3sem/input.txt", "r");
    fscanf(file, "%d\n", &num);
    Line *line = (Line *)malloc(num * sizeof(Line));
    LineScan(num, file, line);
    for(int i = 0; i < num; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            sleep(line[i].waittime);
            execvp(line[i].token[0], line[i].token);
        }
    }
    free(line);
    return 0;
}
