//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <stdio.h>
//
//int main(){
//	int fd, result;
//	size_t size;
//	char resstring[14];
//	char name[]="aaa.fifo";
//	(void)umask(0);
//	if(mknod(name, S_IFIFO | 0666, 0) < 0)
//	{
//		printf("Can\'t create FIFO\n");
//		exit(-1);
//	}
//
//	if((result = fork()) < 0)
//	{
//		printf("Can\'t fork child\n");
//		exit(-1);
//	}
//	else if (result > 0)
//	{
//		if((fd = open(name, O_WRONLY)) < 0){
//			printf("Can\'t open FIFO for writting\n");
//			exit(-1);
//		}
//		size = write(fd, "Hello, world!", 14);
//
//		if(size != 14){
//			printf("Can\'t write all string to FIFO\n");
//			exit(-1);
//		}
//		close(fd);
//		printf("Parent exit\n");
//
//		}
//	else {
//		if((fd = open(name, O_RDONLY)) < 0){
//		printf("Can\'t open FIFO for reading\n");
//		exit(-1);
//		}
//		size = read(fd, resstring, 14);
//		if(size < 0){
//		printf("Can\'t read string\n");
//		exit(-1);
//		}
//		printf("%s\n",resstring);
//		close(fd);
//		}
//	return 0;
//}
//

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
//	line[i].token[0] = (char*)malloc(20 * sizeof(char));
//        line[i].token[1] = (char*)malloc(20 * sizeof(char));
//        strcpy(line[i].token[0], "ololo");
//        strcpy(line[i].token[1], "5");
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

