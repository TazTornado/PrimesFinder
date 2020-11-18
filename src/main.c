#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "subrange.h"
// #include "mysort"

int main(int argc, char* argv[]){
	printf("Hello this is the root.\n\n");


////////////////////* Make necessary checks and handle program parameters *////////////////////

	int lflag = 0, uflag = 0, wflag = 0;
	int ub, lb, numOfChildren;
	while(--argc){ 
		if(strcmp(*argv, "-l") == 0){
			if(lflag == 0){
				lb = atoi(*(argv + 1));
				lflag = 1;
			} else {
				printf("Invalid argument.\nUsage: ./myprime -l lb -u ub -w numOfChildren\n\n");
				exit(EXIT_FAILURE);
			}
		}
		if(strcmp(*argv, "-u") == 0){
			if(uflag == 0){
				ub = atoi(*(argv + 1));
				uflag = 1;
			} else {
				printf("Invalid argument.\nUsage: ./myprime -l lb -u ub -w numOfChildren\n\n");
				exit(EXIT_FAILURE);
			}
		}
		if(strcmp(*argv, "-w") == 0){
			if(wflag == 0){
				numOfChildren = atoi(*(argv + 1));
				wflag = 1;
			} else {
				printf("Invalid argument.\nUsage: ./myprime -l lb -u ub -w numOfChildren\n\n");
				exit(EXIT_FAILURE);
			}
		}
		argv++;
	}
	if((lflag != 1) || (uflag != 1) || (wflag != 1)){
		printf("Usage: ./myprime -l lb -u ub -w numOfChildren\nAll parameters are mandatory, order is flexible.\n");
		exit(EXIT_FAILURE);
	}
	if(ub < lb){
		printf("Upper bound must be larger than lower bound!\n");
		printf("Usage: ./myprime -l lb -u ub -w numOfChildren\n");
		exit(EXIT_FAILURE);
	}

	printf("lb = %d\n", lb);
	printf("ub = %d\n", ub);
	printf("numOfChildren = %d\n", numOfChildren);


////////////////////* Start creating the hierarchy tree of processes *////////////////////

	pid_t chpid;
	int **subranges = getSubRange(lb, ub, numOfChildren);
	char *args[4];	// vector of inline parameters for execv calls

	for(int i = 0; i < numOfChildren; i++){
		chpid = fork();
		switch(chpid){
			case -1:	// fork failure
				perror("Failed to fork");
				exit(1);

			case 0:		// child clause
				// printf("Child process with pid %lu.\n", (long)getpid());
				args[0] = malloc(sizeof("Inodes"));		// does not require free() cause of exec
				args[0] = "Inodes";
				args[1] = malloc(10*sizeof(char));
				sprintf(args[1], "%d", subranges[i][0]); // append lower bound to vector of args for i-node
				args[2] = malloc(10*sizeof(char));
				sprintf(args[2], "%d", subranges[i][1]); // append upper bound to vector of args for i-node				
				args[3] = NULL;
				printf("Args for execv: %s, %s, %s\n", args[0], args[1], args[2]);
				if(execvp("./InnerNodes/Inodes", &args[0]) == -1){
					perror("Failed to exec Inodes");
					exit(1);
				};
				break;

			default:	// parent clause
				// printf("Parent process with pid %lu.\n", (long)getpid());
				break;
		}
	}		


////////////////////////////////////////* Free all used space and exit *////////////////////////////////////////

	// free(args[1]);
	// free(args[2]);
	for(int i = 0; i < numOfChildren; i++)
		free(subranges[i]);
	free(subranges);
	printf("Exiting now..\n");

	return 0;
}