#pragma once

typedef struct InfoChunk{   // a chunk to be written in pipes
    int prime;              
    double time;            // the time of search for particular prime
    double total_time;      // total time including previous numbers in range
} InfoChunk;