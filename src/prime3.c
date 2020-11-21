#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <math.h>

#define YES '1'
#define NO '0'


// O(n*log(logn))
char *EratosthenesSieve(int lb, int ub){
	int i = 0, j = 0;
////////////////// Find the primes in [2..sqrt(ub)] //////////////////
	int new_upper = (int)sqrt(ub) + 1;
	char *sieve = malloc((ub+2) * sizeof(char));			// a sieve the size of given range, with boolean values
	memset(sieve, YES, ub+1);      			// at first all numbers are considered primes => set to true
	memset(sieve + (ub+1), '\0', 1);		// only to copy this as a string later


	sieve[0] = sieve[1] = NO;		// exclude 0 and 1 from primes
	for(i = 2; i <= new_upper; i++){	// up to sqrt(ub)
		 if(sieve[i] == YES){
			 for(j = i*i; j <= ub; j+=i){
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
		int lb=0, ub=0, i=0;

		if ( (argc != 3) ){
				printf("usage: prime3 lb ub\n");
				exit(1); }

		lb=atoi(argv[1]);
		ub=atoi(argv[2]);

		if ( ( lb<1 )  || ( lb > ub ) ) {
				printf("usage: prime3 lb ub\n");
				exit(1); }

		char *marked = EratosthenesSieve(lb, ub);
		for(i = 0; i < (ub - lb + 1); i++){
			if(marked[i] == YES)
				printf("%d\n", (i + lb));
		}

		free(marked);
}