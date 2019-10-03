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
    char* string;
} Line;

char* timeout_s = "timeout";
char* five = "5";

void split(char* str, Line *line, int i){
		int k = 2;
		char* delim = " \n";
        char* first = strtok(str, delim);
        line[i].waittime = atoi(first);
        first = strtok(NULL, delim);
        while (first != NULL){
        	line[i].token[k] = first;
            first = strtok(NULL, delim);
            ++k;
        }
        line[i].token[k] = NULL;
}

void LineScan(int num, FILE *file, Line *line){
	for(int i = 0; i < num; i++)
    {
        line[i].token = (char**)malloc(100 * sizeof(char*));
        line[i].string = (char*)malloc(100 * sizeof(char));
        fgets(line[i].string, 100, file);
        line[i].token[0] = timeout_s;
        line[i].token[1] = five;
        split(line[i].string, line, i);
    }
}

int main()
{
    int num;
    FILE *file = fopen("../input.txt", "r");
    fscanf(file, "%d\n", &num);
    Line *line = (Line *)malloc(num * sizeof(Line));
    LineScan(num, file, line);
    fclose(file);

    for(int i = 0; i < num; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            sleep(line[i].waittime);
            execvp("timeout", line[i].token);
        }
    }

    int number = 0, status = 0;
    for (;;)
    {
    	status = 0;
    	pid_t pid = waitpid(-1, &status, WNOHANG);
    	if (pid)
    	{
    		if ((status >> 8) & 255)
    		{
    			printf("process %d was killed by timeout\n", pid);
    		}
    		else
			{
    			printf("process %d was terminated (not killed)\n", pid);
			}
    		number++;
    	}

    	if (number == num)
    		break;
    }

    for (int i = 0; i < num; i++)
    {
    	free(line[i].string);
    	free(line[i].token);
    }
    free(line);


    return 0;
}
