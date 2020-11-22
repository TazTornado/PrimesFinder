#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/times.h>
#include <unistd.h>

#include "datatypes.h"

#define YES 1
#define NO  0


// O(n^2)
int prime(int n){
		int i;
		if (n==1) return(NO);
		for (i=2 ; i<n ; i++)
				if ( n % i == 0) return(NO);
		return(YES);
}

int main(int argc, char *argv[]){
	int lb=0, ub=0, i=0;

	if ( (argc != 3) ){
		printf("usage: prime1 lb ub\n");
		exit(1); }

	lb=atoi(argv[1]);
	ub=atoi(argv[2]);

	if ( ( lb<1 )  || ( lb > ub ) ) {
		printf("usage: prime1 lb ub\n");
		exit(1); }

	double t1, t2, total = 0.0;
    struct tms tb1, tb2;
    double ticspersec;
	ticspersec = (double) sysconf(_SC_CLK_TCK);

    t1 = (double) times(&tb1);
	for (i=lb ; i <= ub ; i++){
		if ( prime(i)==YES ){
			t2 = (double) times(&tb2);
			InfoChunk chunk;		// create struct to write to pipe
			chunk.prime = i;
			chunk.time = ((t2 - t1) / ticspersec) * 1000.0;	// convert sec to msec
			total += chunk.time;							// calculating total search time prime-by-prime
			chunk.total_time = total;						// only the last chunk's total time will be used
			printf("prime struct: %d, %f, %f \n", chunk.prime, chunk.time, chunk.total_time);
		}
	}
	printf("\n");
}
