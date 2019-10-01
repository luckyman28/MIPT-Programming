#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int* CreateArray(int vertices){
	int* tree = (int*) malloc(sizeof(int) * (vertices + 1));
        int elem, num = 0;
	printf("Now type the whole tree (try 2 -1 2 3 3 3 0)\n");
        scanf("%d ", &elem);
        tree[0] = elem;
        while(elem != 0){ //последний элемент ввода 0 - сигнализирует о конце входных данных, в дерево он не входит
                num ++;
                scanf("%d", &elem);
                tree[num] = elem;
        }
	return (tree);
}


int FindTop(int* tree){
	int top = 0;
        while(tree[top] != -1){
                top++;
        }
        top++;
	return top;
}

int ArrayLength(int* tree){
	int i = 0;
	while(tree[i] != 0){
		i++;
	}
	return i;
}

void MakePtree(int* tree, int vertex, int len, int maxbabies){
	pid_t* babies = (pid_t*) malloc(sizeof(pid_t) * (maxbabies + 1));
	int bnumber = 0;
	for(int i = 0; i < len; ++i){
		if (tree[i] == vertex){
			pid_t pid = fork();
			if (pid == 0) {
				sleep(1); //гарантия того, что  все процессы какое-то время вместе поработают
				printf("%d had a baby %d\n", getppid(), getpid());
				int j = i + 1; //вводим j, чтобы не сбить i
				MakePtree(tree, j, len, maxbabies);
			}
			else{
				babies[bnumber] = pid;
				++bnumber;
			}
		}
	}
	if (bnumber != 0){
		for (int i = 0; i < bnumber; ++i){
			int* status;
			waitpid(babies[i], status, 0);
			sleep(1);
			printf("%d died. He was a son of %d. Press F to pay respect\n", babies[i], getpid());
		}
	}
	sleep(1);
	free(babies);
	exit(57);
}

int main() {
	printf("How many vertices does the graph consist of? (try 6)\n");
	int vertices, maxbabies;
	scanf("%d", &vertices);
        printf("What is the maximum number of babies for one vertex? (try 3)\n");	
	scanf("%d", &maxbabies);
	int* tree = CreateArray(vertices);
	int top = FindTop(tree);
	int len = ArrayLength(tree);
	MakePtree(tree, top, len, maxbabies); 
	return 0;


}
