/////////////////// Program to overlay the job of I-nodes in tree ///////////////////
/* Its job is to create the W-nodes and gather their results through pipes. Then, it sorts gathered data and returns them to the root*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/times.h> 
#include <sys/types.h>
#include <sys/wait.h>

#include "subrange.h"
#include "datatypes.h"

#define BUFFSIZE sizeof(InfoChunk) 

void USR1_handler(){
    signal(SIGUSR1, USR1_handler);
    kill(getppid(), SIGUSR1);	// 'forward' signal to parent process => root
}


int main(int argc, char *argv[]){

////////////////////* Handle program parameters *////////////////////


	if((argc < 4) || (argc > 6)){       // arguments are too few or too many
		printf("Usage: ./Inodes lb ub numOfChildren myindex pipefd\n(myindex, and pipefd are optional)\n");
		exit(1);
	}
	// printf("\nargc = %d\n", argc);
	int lb = atoi(argv[1]);     	// lower bound
	int ub = atoi(argv[2]);     	// upper bound
	int bfactor = atoi(argv[3]);	// branching factor/number of children
	int myindex = atoi(argv[4]);	// myindex shows which Inode this is in the intermediate level of hierarchy
	int writefd = STDOUT_FILENO;

	if(argc == 6)      				// optional argument pipe_fd
		writefd = atoi(argv[5]);	// if there is a pipe from parent process, then save the write end

	if(ub < lb){
		printf("Upper bound must be greater than lower bound!\n");
		printf("Usage: ./Inodes lb ub numOfChildren myindex pipefd\n(myindex, and pipefd are optional)\n");
		exit(EXIT_FAILURE);
	}



////////////////////////////////////////* Create the worker processes *////////////////////////////////////////

	signal(SIGUSR1, USR1_handler);
	pid_t chpid;

	while((ub-lb+1) < bfactor)	// in case the range is too small to divide into bfactor processes
		bfactor--;				// reduce bfactor until a rational number is reached
	int **subranges = getSubRange(lb, ub, bfactor);		// divide my range into bfactor subranges for my children
	char *args[5];					// vector of inline parameters for execv calls
	int pipefds[bfactor][2];		// all pipe file descriptors for communication will be kept here
	int tempfd[2];					// this will temporarily hold what pipe() returns
	InfoChunk readbuf;				
	int status;						// for use in wait()

	for(int i = 0; i < bfactor; i++){
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
				args[0] = malloc(sizeof("prime*"));			// does not require free() cause of exec
				args[1] = malloc(10*sizeof(char));
				sprintf(args[1], "%d", subranges[i][0]);	// append lower bound to vector of args for i-node
				args[2] = malloc(10*sizeof(char));
				sprintf(args[2], "%d", subranges[i][1]);	// append upper bound to args		
				args[3] = malloc(10*sizeof(char));			
				sprintf(args[3], "%d", pipefds[i][1]);		// append writefd to args	
				args[4] = NULL;

				int assigned_finder = (i + myindex * bfactor) % 3;	// decision takes into account the "global" order of assigned algorithms
				switch(assigned_finder){		// decide which prime-finding algorithm to assign to this child

					case 0: 
						args[0] = "prime1";		// prepare executable name for exec
						if(execvp("../bin/prime1", &args[0]) == -1){
							perror("Failed to exec prime1");
							exit(1);
						};

					case 1:
						args[0] = "prime2";		// prepare executable name for exec
						if(execvp("../bin/prime2", &args[0]) == -1){
							perror("Failed to exec prime2");
							exit(1);
						};

					case 2:
						args[0] = "prime3";		// prepare executable name for exec
						if(execvp("../bin/prime3", &args[0]) == -1){
							perror("Failed to exec prime3");
							exit(1);
						};
				}
				break;

			default:	// parent clause
				close(pipefds[i][1]);	// close write end
				break;
		}
	}	

		while((chpid = wait(&status)) > 0); 	// wait for all workers to finish


////////////////////////////////////////* Read and Forward Data to Root *////////////////////////////////////////

		int read_data = 0;					
		for(int i = 0; i < bfactor; i++){
			while((read_data = read(pipefds[i][0], &readbuf, BUFFSIZE)) != 0){
				if(write(writefd, &readbuf, BUFFSIZE) == -1){
					printf("Inode %d failed to write in %d\n", myindex, writefd);
					perror("write failure");
					exit(1);
				}
			}
			close(pipefds[i][0]);		
		}

////////////////////////////////////////* Free all used space and exit *////////////////////////////////////////

	for(int i = 0; i < bfactor; i++)
		free(subranges[i]);
	free(subranges);

	// printf("Inode exiting now..\n\n");
	return 0;
}