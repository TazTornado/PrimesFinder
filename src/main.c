#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dummy.h"

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

	printf("\n\nlb = %d\n", lb);
	printf("ub = %d\n", ub);
	printf("numOfChildren = %d\n", numOfChildren);
	printf("Exiting now..\n");

	return 0;
}