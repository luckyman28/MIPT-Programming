#include "functions.h"



int* ArrCreate (char** PtrArr, int height);
void washer (int* WashArr, FILE* shared, sem_t semid);
void dryer (FILE* table_f, sem_t semid);
int table_size;


int main (int argc, char* argv[])
{
	FILE *input, *shared_wash = NULL, *shared_dry = NULL;
	int height, count = 0;
	int* WashArr;
	char *FileString;
	char** PtrArr;
	if (argc != 2)
	{
		printf("ERROR: wrong amount of arguments\n");
		exit(EXIT_FAILURE);
	}

	sem_t semid = 0; 
	if ( (semid = semget(IPC_PRIVATE, 3, 0666 | IPC_CREAT)) < -1 )
	{
		printf("Error: semget: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}	
	
	check_str (argv[1]);
	table_size = atoi(argv[1]);

	init_empty(semid, table_size);
	init_full(semid);

	input = fopen("dirty_heap.txt", "r");
	check(input);

	height		= FileSize(input);
	FileString	= givememory_char((size_t) height + 1);
	PtrArr		= readtxt(input, &height, FileString);
	height--;
	fclose(input);

	WashArr = ArrCreate (PtrArr, height);
	while (WashArr[count] != -1)
		printf("WashArr is %d\n", WashArr[count++]);

	pid_t pid = fork(); 

	if (pid > 0)
		washer(WashArr, shared_wash, semid);
	else if (pid == 0)
		dryer(shared_dry, semid);
	else
	{
		printf("error of fork\n");
		exit(EXIT_FAILURE);
	}
	free(FileString);
	free(WashArr);
	free(PtrArr);
	return 0;
}


int* ArrCreate (char** PtrArr, int height)
{
	int count = 0, words = 0, checker = 0, com;
	FILE* commands = fopen("dish_type.h", "r");
	char* FileString;
	char** ComArr;
	check (commands);
	com = FileSize (commands);
	FileString = givememory_char((size_t) com + 1);
	ComArr = readtxt(commands, &com, FileString);
	com--;
	fclose(commands);
	int* arr = givememory_int ((size_t) com);
	for (count = 0; count < height; count++)
	{
		char** string = split(PtrArr[count], &words);
		if (words != 2)
		{
			printf("ERROR: wrong style of input file words amount is %d count is %d \n", words, count);
			exit(EXIT_FAILURE);
		}
		#define TYPE(name, num, wash, dry)		    \
				if (strcmp(string[0], #name) == 0)  \
		{		    				    \
			arr[num] = atoi(string[1]);                 \
			checker++;				    \
		}
		#include "dish_type.h"
		#undef TYPE
		if (checker != 1)
		{
			printf("no such dish type\n");
			exit(EXIT_FAILURE);
		}
		free(string);
		checker = 0;
	}
	printf("com is %d\n", com);
	arr[com] = -1;
	free(FileString);
	free(ComArr);
	return arr;
}

void washer (int* WashArr, FILE* shared_wash, sem_t semid)
{
	int count = 0;
	while (WashArr[count] != -1)
	{
		while (WashArr[count] > 0)
		{
			//printf("_!_!_!_washer: empty: %d: %s\n", semctl(semid, EMPTY, GETVAL), strerror(errno));
			wait_empty(semid);
				//ifnotmutex+setmutex
			wait_mutex(semid);
			shared_wash = fopen("table", "a");
			if (1 > fprintf(shared_wash, "%d\n", count))
				printf("FPRINTF ERROR %s", strerror(errno));
			WashArr[count]--;
			mutex_release(semid);
#define TYPE(name, num, wash, dry)				\
			if (count == num)					\
			{									\
				sleep(wash);					\
				printf("i washed %s\n", #name);	\
				fclose(shared_wash);			\
			}
			#include "dish_type.h"
#undef TYPE
				//increasefull
			inc_full(semid);
		}
		count++;
	}
	shared_wash = fopen("table", "a");
	fprintf(shared_wash, "%d\n", END);
	inc_full(semid);
	fclose(shared_wash);
	wait(NULL);
}

void dryer (FILE* shared_dry, sem_t semid)
{
	int bias   = 0;
	int offset = 0;
	while (1)
	{
		int  value = -1;
		//ifnotempty
		wait_full(semid);
		//ifnotmutex+setmutex
		wait_mutex(semid);
		shared_dry  = fopen("table", "r");
		check(shared_dry);
		fseek(shared_dry, bias, SEEK_SET);
		fscanf(shared_dry, "%d%n", &value, &offset);
		bias += offset + 1;
		mutex_release(semid);

#define TYPE(name, num, wash, dry)								\
	if (value == num)											\
	{															\
		sleep(dry);												\
		printf("i dried %s\n", #name);							\
		fclose(shared_dry);										\
	}															
#include "dish_type.h"		
#undef TYPE
	if (value == END)										
	{															
		printf("washer finished, and the table is clean\n");	
		break;												
	}
		//releasemutex
		//decrease_full;
		inc_empty(semid);
	}
	system("rm table");
	exit(EXIT_SUCCESS);
}

