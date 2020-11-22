#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/times.h> 
#include <sys/wait.h>
#include "subrange.h"
#include "datatypes.h"

#define BUFFSIZE sizeof(InfoChunk)
int usr1_received = 0;

void USR1_handler(){
    signal(SIGUSR1, USR1_handler);
						/////////////////////////////////////////////
	 usr1_received++;	// increment given counter of USR1 signals //
						// received in invoking process => root    //
						/////////////////////////////////////////////
}


int main(int argc, char* argv[]){


////////////////////* Make necessary checks and handle program parameters *////////////////////

	int lflag = 0, uflag = 0, wflag = 0;
	int ub, lb, numOfChildren;
	char *args[7];	// vector of inline parameters for execv calls

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
				args[3] = strdup(*(argv + 1));
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
		printf("Upper bound must be greater than lower bound!\n");
		printf("Usage: ./myprime -l lb -u ub -w numOfChildren\n");
		exit(EXIT_FAILURE);
	}


////////////////////* Start creating the hierarchy tree of processes *////////////////////

	signal(SIGUSR1, USR1_handler);
	pid_t chpid;
	int **subranges = getSubRange(lb, ub, numOfChildren);
	int pipefds[numOfChildren][2];		// all pipe file descriptors for communication will be kept here
	int tempfd[2];						// this will temporarily hold what pipe() returns
	InfoChunk readbuf;
	int status;							// for use in wait()

	for(int i = 0; i < numOfChildren; i++){
		if(pipe(tempfd) == -1){
			printf("Failed to open pipe for worker %d ", i);
			perror("");
			exit(1);
		}

		pipefds[i][0] = tempfd[0];		// save readfd
		pipefds[i][1] = tempfd[1];		// save writefd

		chpid = fork();
		switch(chpid){
			case -1:	// fork failure
				perror("Failed to fork");
				exit(1);

			case 0:		// child clause
				args[0] = malloc(sizeof("Inode"));		// does not require free() cause of exec
				args[0] = "Inode";
				args[1] = malloc(10*sizeof(char));
				sprintf(args[1], "%d", subranges[i][0]);	// append lower bound to vector of args for i-node
				args[2] = malloc(10*sizeof(char));
				sprintf(args[2], "%d", subranges[i][1]);	// append upper bound to args	
				/* args[3] is the numOfChildren */
				args[4] = malloc(10*sizeof(char));
				sprintf(args[4], "%d", i);					// append to args, the index of Inode in level 1 of hierarchy 	
				args[5] = malloc(10*sizeof(char));			
				sprintf(args[5], "%d", pipefds[i][1]);		// append writefd to args	
				args[6] = NULL;

				if(execvp("../bin/Inode", &args[0]) == -1){
					perror("Failed to exec Inode");
					exit(1);
				};
				break;

			default:	// parent clause
				close(pipefds[i][1]);	// close write end
				break;
		}
	}		

	while((chpid = wait(&status)) > 0); 	// wait for all workers to finish

////////////////////////////////////////* Gather Information and Print Data *////////////////////////////////////////
	
	int workers_activated = 0;
	int read_data = 0;
	double max_worker = 0.0, min_worker = 3000.0;					

	printf("Primes in [%d,%d] are:\n", lb, ub);
	printf("-------------------------------\n");

	for(int i = 0; i < numOfChildren; i++){
			while((read_data = read(pipefds[i][0], &readbuf, BUFFSIZE)) != 0){
				printf("Result: %d	Time: %f msecs\n", readbuf.prime, readbuf.time);
				if(readbuf.total_time < min_worker)
					min_worker = readbuf.total_time;
				if(readbuf.total_time > max_worker)
					max_worker = readbuf.total_time;
				workers_activated++;
			}
			close(pipefds[i][0]);		
		}
		printf("-------------------------------\n");

	printf("Min Time for Workers : %f msecs\n", min_worker);
	printf("Max Time for Workers : %f msecs\n", max_worker);
	printf("Num of USR1 Received : %d/%d\n", usr1_received, workers_activated);

////////////////////////////////////////* Free all used space and exit *////////////////////////////////////////

	free(args[3]);
	for(int i = 0; i < numOfChildren; i++)
		free(subranges[i]);
	free(subranges);

	return 0;
}