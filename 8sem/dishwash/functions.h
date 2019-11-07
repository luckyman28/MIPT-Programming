#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <wait.h>

#define MUTEX 0
#define FULL  1
#define EMPTY 2
#define END 1000

typedef int sem_t;

typedef struct sembuf Sem_op;

void mutex_up(Sem_op* so);
void mutex_down(Sem_op* op);
void wait_for_zero(Sem_op* so, unsigned short num);
void inc_sem(Sem_op* so, unsigned short num);
void dec_sem(Sem_op* so, unsigned short num);
void set_sem(Sem_op* so, unsigned short num);
void init_full(sem_t semid);
void init_empty(sem_t semid, int max);
void mutex_set(sem_t semid);
void mutex_release(sem_t semid);
void wait_mutex(sem_t semid);
void wait_full(sem_t semid, int max);
void wait_empty(sem_t semid);
void inc_full(sem_t semid);
void inc_empty(sem_t semid);
void dec_full(sem_t semid);
void dec_empty(sem_t semid);
void initialize_empty(sem_t semid, int size);
int* givememory_int (size_t size);
char** givememory_str (size_t size);
char* givememory_char (size_t size);
double* givememory_double (size_t size);
int FileSize (FILE* file);
void check (FILE* text);
char** readtxt(FILE* text, int* height, char* FileString);
void check_str (char* word);
char** split (char* s, int* length);

int FileSize (FILE* file)
{
	assert(file);	
	long int length;	
	if (fseek(file, 0, SEEK_END) == -1)
	{
		perror("fseek crashed");
		exit(EXIT_FAILURE);
	}
	length = ftell(file);
	rewind(file);
	if (length <= 0)
	{
		printf("error of reading");
		exit(EXIT_FAILURE);
	}
	return (int) length;
}

char** readtxt(FILE* text, int* height, char* FileString)
{
	assert(text);
	assert(height);
	assert(FileString);
	int i, j = 1, count = 0;
	size_t k;
	char** PtrArr;	
	if ((k = fread(FileString, sizeof(char), (size_t) *height, text)) != (size_t) *height)
	{
		printf("read wrong");
		exit(EXIT_FAILURE);
	}
	FileString[*height] = '\n';
	for (i = 0;i < *height + 1; i++)
	{
		if (FileString[i] == '\n') 
		{
			count++;
		}
	}
	FileString[*height] = '\0';
	if (!(PtrArr = (char**) calloc((size_t) count, sizeof(char*))))
	{
		perror("calloc readtxt: error:\n");
		exit(EXIT_FAILURE);
	}
	PtrArr[0] = &(FileString[0]);
	for (i = 0; i < *height; i++)
	{
		if (FileString[i] == '\n')
		{
			FileString[i] = '\0';
			PtrArr[j] = &(FileString[i + 1]);
			j++;
		}
	}
	*height = count;
	return PtrArr;
}

int* givememory_int (size_t size)
{
	int* IntArr;
	if (!(IntArr = (int*) calloc (size + 1, sizeof(int))))
	{
		perror("IntArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	IntArr[size] = '\0';
	return IntArr;
}

char** givememory_str (size_t size)
{
	char** PtrArr;
	if (!(PtrArr = (char**) calloc (size, sizeof(char*))))
	{
		perror("PtrArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	return PtrArr;
}

char* givememory_char (size_t size)
{
	char* CharArr;
	if (!(CharArr = (char*) calloc (size + 1, sizeof(char))))
	{
		perror("CharArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	CharArr[size] = '\0';
	return CharArr;
}

double* givememory_double (size_t size)
{
	double* DobArr;
	if (!(DobArr = (double*) calloc (size + 1, sizeof(double))))
	{
		perror("CharArr: calloc error\n");
		exit(EXIT_FAILURE);
	}
	DobArr[size] = '\0';
	return DobArr;
}

void check (FILE* text)
{
	if (text == NULL)
	{
		printf("fopen: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void check_str (char* word)
{
	int count = 0;
	while (word[count] != '\0')
	{
		if (isdigit(word[count]) == 0)
		{
			printf("arguments should have format integer, but it is '%s'\n", word);
			exit(EXIT_FAILURE);
		}
		count++;
	}
}

char** split (char* s, int* length)
{
	int counter = 1, i = 0;
	char** head;
	char* point;
	errno = 0;
	if (!(head = (char**) calloc (1, sizeof(char*))))
	{
		printf("realloc: error: %s\n", strerror(errno));
		exit(-1);
	}
	point = strtok (s, " ");
	while (point != NULL)
	{
		if (i + 2 >= counter)
		{
			counter *= 2;
			if (!(head = (char**) realloc (head,  counter * sizeof(char*))))
			{
				printf("realloc: error: %s\n", strerror(errno));
				exit(-1);
			}
		}
		head[i] = point;
		i++;
		point = strtok (NULL, " ");
	}
	*length = i;
	head[i] = (char*) NULL;
	return head;
}

/* figuring semop structures */
void mutex_up(Sem_op* so)
{
	so->sem_num = MUTEX;
	so->sem_op  = 1;
	so->sem_flg = 0;
}

void mutex_down(Sem_op* so)
{
	so->sem_num = MUTEX;
	so->sem_op  = -1;
	so->sem_flg = 0;
}

void wait_for_zero(Sem_op* so, unsigned short num)
{
	so->sem_num = num;
	so->sem_op  = 0;
	so->sem_flg = 0;
}

void inc_sem(Sem_op* so, unsigned short num)
{
	so->sem_num = num;
	so->sem_op	= 1;
	so->sem_flg = 0;
}

void dec_sem(Sem_op* so, unsigned short num)
{
	so->sem_num = num;
	so->sem_op  = -1;
	so->sem_flg = 0;
}

void set_sem(Sem_op* so, unsigned short num)
{
	so->sem_num = num;
	so->sem_op  = 30;
	so->sem_flg = 0;
}


/* functions including semop */

/* wait + set*/
void wait_mutex(sem_t semid)
{
	Sem_op ops[2] = {{MUTEX, 0, 0}, {MUTEX, 1, 0}};
	semop(semid, ops, 2);
}

void mutex_release(sem_t semid)
{
	Sem_op so = {};
	mutex_down(&so);
	semop(semid, &so, 1);
}

void init_empty(sem_t semid, int max)
{
	semctl(semid, EMPTY, SETVAL, max);
}

void init_full(sem_t semid)
{
	semctl(semid, FULL, SETVAL, 0);
}

/*waits until at least one slot is free*/
void wait_empty(sem_t semid)
{
	Sem_op op = {EMPTY, -1, 0};
	semop(semid, &op, 1);
}


/* waits until at least one dish is at the table*/
void wait_full(sem_t semid)
{
	Sem_op op = {FULL, -1, 0};

	semop(semid, &op, 1);
}

void inc_full(sem_t semid)
{
	Sem_op so = {};
	inc_sem(&so, FULL);
	semop(semid, &so, 1);
}

void inc_empty(sem_t semid)
{
	Sem_op so = {EMPTY, 1, 0};
	semop(semid, &so, 1);
}

void dec_full(sem_t semid)
{
	Sem_op so = {};
	dec_sem(&so, FULL);
	semop(semid, &so, 1);
}

void dec_empty(sem_t semid)
{
	Sem_op so = {EMPTY, -1, 0};
	semop(semid, &so, 1);
}
