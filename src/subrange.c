#include <stdio.h>
#include <stdlib.h>
#include "subrange.h"


// a function that evenly distributes a subrange into n subranges
// returns a dynamic list of n integer pairs
// that represent the lower and upper bound of each subrange

int **getSubRange(int lower, int upper, int n){
    if(lower > upper){
        printf("getSubRange() failed. Upper bound must be larger than lower.\n");
        return NULL;
    }	

	int **subranges = malloc(n*sizeof(int*)); // alloc space for list of pairs to return    
	for(int i = 0; i < n; i++){                  	
		subranges[i] = malloc(2*sizeof(int));			
	}													

	int range_size = upper - lower + 1;	    // length of given range in ints
	int subrange_size = range_size / n - 1;	// minimum subrange length 
	int extra_bit = range_size % n; 		// some subranges will grow if there is remainder
    int newlow = lower;                     // first lower bound will be equal to lower
    
    for(int i = 0; i < n; i++){
        subranges[i][0] = newlow;
        if(extra_bit > 0){     // as long as there is extra bit, grow this pair by 1
            subranges[i][1] = subranges[i][0] + subrange_size + 1;  // calculate upper range
            extra_bit -= 1; 
        } else {
            subranges[i][1] = subranges[i][0] + subrange_size;      // calculate upper range       
        }
        newlow = subranges[i][1] + 1;        
    }

    return subranges;
}