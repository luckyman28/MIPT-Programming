#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

void Find(int leftdepth, char* path, char* filename){
	DIR *ptr = opendir(path);
	struct dirent *ent;
	char newpath[300] = {};
	while ((ent = readdir(ptr)) != NULL){
		if (ent->d_type == DT_REG){
			if(strcmp(ent->d_name, filename) == 0){
				printf("we found file %s, the path is %s\n", filename, path);
			}
		}
		if(ent->d_type == DT_DIR){
			sprintf(newpath, "%s/%s", path, ent->d_name);
			if(leftdepth - 1 > 0){
				Find(leftdepth - 1, newpath, filename);
			}
		}
	}
	closedir(ptr);
}

int main(int argc, char ** argv){
	Find(atoi(argv[1]), argv[2], argv[3]);
}



