#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <sys/times.h>
#include "datatypes.h"

#define YES '1'
#define NO '0'
#define BUFFSIZE sizeof(InfoChunk)



// O(n*log(logn))
char *EratosthenesSieve(int lb, int ub){
	int i = 0, j = 0;
////////////////// Find the primes in [2..sqrt(ub)] //////////////////
	int new_upper = (int)sqrt(ub) + 1;
	char *sieve = malloc((ub+2) * sizeof(char));			// a sieve the size of given range, with boolean values
	memset(sieve, YES, ub+1);      			// at first all numbers are considered primes => set to true
	memset(sieve + (ub+1), '\0', 1);		// only to copy this as a string later


	sieve[0] = sieve[1] = NO;			// exclude 0 and 1 from primes
	for(i = 2; i <= new_upper; i++){	// up to sqrt(ub)
		 if(sieve[i] == YES){
			 for(j = i*i; j <= ub; j+=i){	// up to ub
				sieve[j] = NO;
			 }
		 }
	}


////////////////// Keep the primes in [lb..ub] //////////////////

	char *primes_in_range = malloc((ub-lb+2) * sizeof(char));	// a vector indicating primes and non-primes in [lb, ub]
	memset(primes_in_range + (ub-lb+1), '\0', 1);      			// make it a string to use strcpy
	primes_in_range = strcpy(primes_in_range, sieve + lb);		// only return the sieve segment contained in range
	free(sieve);
	return primes_in_range;
}



int main(int argc, char *argv[]){
	int lb=0, ub=0 ,i=0; 
	int writefd = STDOUT_FILENO;	// initialized to the fd of stdout, in case there is no pipefd arg

	if ( (argc < 3) ){
		printf("usage: prime3 lb ub\n");
		exit(1); 
	}

	if(argc == 4){
		writefd = atoi(argv[3]);
	}

	lb=atoi(argv[1]);
	ub=atoi(argv[2]);

	if ( ( lb<1 )  || ( lb > ub ) ) {
		printf("usage: prime3 lb ub\n");
		exit(1); }


	double t1, t2, total, sieving_time = 0.0;
	struct tms tb1, tb2;
	double ticspersec;
	ticspersec = (double) sysconf(_SC_CLK_TCK);

	t1 = (double) times(&tb1);
	char *marked = EratosthenesSieve(lb, ub);
	t2 = (double) times(&tb2);
	total = sieving_time = ((t2 - t1) / ticspersec) * 1000.0;	// sieving only happens once,
																// thus contained once in total time
	t1 = (double) times(&tb1);	
	for(i = 0; i < (ub - lb + 1); i++){
		if(marked[i] == YES){
			t2 = (double) times(&tb2);

			InfoChunk chunk;		// create struct to write to pipe
			chunk.prime = i + lb;
			chunk.time = (((t2 - t1) / ticspersec) * 1000.0) + sieving_time;	// prime time = sieving time + checking-if-prime time
			total += chunk.time - sieving_time;							// calculating total search time prime-by-prime
			chunk.total_time = total;						// only the last chunk's total time will be used
			
			if(write(writefd, &chunk, BUFFSIZE) == -1){
				printf("Worker prime1 failed to write in %d\n", writefd);
				perror("write failure");
				exit(1);
			}
		}
	}

	close(writefd);
	kill(getppid(), SIGUSR1);
	free(marked);
	// printf("Prime3 worker exiting..\n");
	return 0;

}