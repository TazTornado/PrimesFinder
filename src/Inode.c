/////////////////// Program to overlay the job of I-nodes in tree ///////////////////
/* Its job is to create the W-nodes and gather their results through pipes. Then, it sorts gathered data and returns them to the root*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/times.h> 
// #include <sys/types.h>
#include "subrange.h"
// #include "mysort.h"


void USR1_handler(){
    signal(SIGUSR1, USR1_handler);
    kill(getppid(), SIGUSR1);	// 'forward' signal to parent process => root
}


int main(int argc, char *argv[]){
	// printf("Hello this is the Intermediate nodes code file.\n");

////////////////////* Handle program parameters *////////////////////

	if((argc < 4) || (argc > 7)){       // arguments are too few or too many
		printf("Usage: ./Inodes lb ub numOfChildren myindex pipefd\n(myindex, and pipefd are optional)\n");
		exit(1);
	}
	// printf("\nargc = %d\n", argc);
	int lb = atoi(argv[1]);     	// lower bound
	int ub = atoi(argv[2]);     	// upper bound
	int bfactor = atoi(argv[3]);	// branching factor/number of children
	// int writefd = -1;
	int myindex = atoi(argv[4]);	// myindex shows which Inode this is in the intermediate level of hierarchy
	printf("\n\nThis Inode's args: lb = %d, ub = %d, bfactor = %d, myindex = %d\n", lb, ub, bfactor, myindex);

	// if(argc == 5)      // optional argument pipe_fd
	// 	writefd = atoi(argv[4]);	// if there is a pipe from parent process, then save the write end

	if(ub < lb){
		printf("Upper bound must be greater than lower bound!\n");
		printf("Usage: ./Inodes lb ub numOfChildren myindex pipefd\n(myindex, and pipefd are optional)\n");
		exit(EXIT_FAILURE);
	}



////////////////////////////////////////* Create the worker processes *////////////////////////////////////////

	pid_t chpid;

	while((ub-lb+1) < bfactor)	// in case the range is too small to divide into bfactor processes
		bfactor--;				// reduce bfactor until a rational number is reached
	// printf("GONNA CREATE %d WORKER(S)\n", bfactor);
	int **subranges = getSubRange(lb, ub, bfactor);		// divide my range into bfactor subranges for my children
	char *args[5];	// vector of inline parameters for execv calls


	for(int i = 0; i < bfactor; i++){
		// TODO: create pipe here and before fork add to a vector of children pids and readfds of pipes
		chpid = fork();
		switch(chpid){
			case -1:	// fork failure
				perror("Failed to fork");
				exit(1);

			case 0:		// child clause
				// printf("Child process with pid %lu.\n", (long)getpid());
				args[0] = malloc(sizeof("prime*"));		// does not require free() cause of exec
				args[1] = malloc(10*sizeof(char));
				sprintf(args[1], "%d", subranges[i][0]);	// append lower bound to vector of args for i-node
				args[2] = malloc(10*sizeof(char));
				sprintf(args[2], "%d", subranges[i][1]);	// append upper bound to args		
				args[3] = NULL;		// pipe writefd
				args[4] = NULL;

				int assigned_finder = (i + myindex * bfactor) % 3;	// decision takes into account the "global" order of assigned algorithms
				printf("assigned_finder = %d\n", assigned_finder);
				switch(assigned_finder){		// decide which prime-finding algorithm to assign to this child

					case 0: 
						args[0] = "prime1";		// prepare executable name for exec
						printf("Args for execv: %s, %s, %s\n", args[0], args[1], args[2]);	// overlay worker with prime1
						if(execvp("../bin/prime1", &args[0]) == -1){
							perror("Failed to exec prime1");
							exit(1);
						};

					case 1:
						args[0] = "prime2";		// prepare executable name for exec
						printf("Args for execv: %s, %s, %s\n", args[0], args[1], args[2]);	// overlay worker with prime2
						if(execvp("../bin/prime2", &args[0]) == -1){
							perror("Failed to exec prime2");
							exit(1);
						};

					case 2:
						args[0] = "prime3";		// prepare executable name for exec
						printf("Args for execv: %s, %s, %s\n", args[0], args[1], args[2]);	// overlay worker with prime3
						if(execvp("../bin/prime3", &args[0]) == -1){
							perror("Failed to exec prime3");
							exit(1);
						};
				}
				break;

			default:	// parent clause
				// printf("Parent process with pid %lu.\n", (long)getpid());
				break;
		}
	}		

////////////////////////////////////////* Free all used space and exit *////////////////////////////////////////

	// free(args[1]);
	// free(args[3]);
	for(int i = 0; i < bfactor; i++)
		free(subranges[i]);
	free(subranges);
	printf("Exiting now..\n\n");


	return 0;
}