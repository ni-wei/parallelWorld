#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"
/**/
struct pthread_args
{
	unsigned int k;
	block_t *blocks;
	unsigned int nBlocks;
	unsigned int num_threads;
	histogram_t histogram;
};


void * hello (void * ptr)
// void * hello ()
{

	unsigned int i, j;
	struct pthread_args *arg = ptr;
	// unsigned int *out = malloc ( NALPHABET ) ; 
	// unsigned int * myhistogram = malloc ( NALPHABET ) ; 
	// histogram_t histogram = malloc ( sizeof ( histogram ) ) ;
	// build local histogram
	for (i=arg->k; i<arg->nBlocks; i+=arg->num_threads) {
		for (j=0; j<BLOCKSIZE; j++) {
			if (arg->blocks[i][j] >= 'a' && arg->blocks[i][j] <= 'z')
				// (*(out+(arg->blocks[i][j]-'a')))++;
				arg->histogram[arg->blocks[i][j]-'a']++;
			else if(arg->blocks[i][j] >= 'A' && arg->blocks[i][j] <= 'Z')
				// (*(out+(arg->blocks[i][j]-'A')))++;
				arg->histogram[arg->blocks[i][j]-'a']++;
		}
	}
	return arg->histogram;
	// out = arg->histogram;
	// return myhistogram;
	// return NULL;
}


void get_histogram(unsigned int nBlocks,
		   						 block_t *blocks,
		   						 histogram_t histogram,
		   						 unsigned int num_threads) {

	// insert your code here

	pthread_t * thread; struct pthread_args * thread_arg;
	thread = malloc ( num_threads * sizeof ( * thread ) ) ;
	
	thread_arg = malloc ( num_threads * sizeof ( * thread_arg ) ) ;

	// unsigned int i, j;

	
	for ( int k = 0; k < num_threads ; k++){
		thread_arg[k].k = k;
		thread_arg[k].nBlocks = nBlocks;
		thread_arg[k].blocks = blocks;
		thread_arg[k].num_threads = num_threads;

		pthread_create ( thread + k , NULL, &hello , thread_arg + k ) ;
		// pthread_create ( thread + k , NULL, &hello , NULL ) ;
	
	}

	for ( int k = 0; k < num_threads ; k++){
		// unsigned int *out;
		// unsigned int * myhistogram;
		// histogram_t myhistogram = malloc ( sizeof ( *myhistogram ) ) ;
		// myhistogram = histogram;


		// pthread_join ( thread [ k ] , NULL ) ;
		// free(out);
		for ( int idx = 0; idx < NALPHABET ; idx++)
			histogram[idx] += thread_arg[k].histogram[idx];
			// histogram[idx] += out[idx];
			// histogram[idx] += myhistogram[idx];
		// build histogram
		pthread_join ( thread [ k ] , (void *) thread_arg[k].histogram ) ;
		// pthread_join ( thread [ k ] , (void *) &out ) ;
		// pthread_join ( thread [ k ] , (void *) &myhistogram ) ;
		// free ( myhistogram ) ;
/*
		for (i=k; i<nBlocks; i+=num_threads) {
			for (j=0; j<BLOCKSIZE; j++) {
				if (blocks[i][j] >= 'a' && blocks[i][j] <= 'z')
					histogram[blocks[i][j]-'a']++;
				else if(blocks[i][j] >= 'A' && blocks[i][j] <= 'Z')
					histogram[blocks[i][j]-'A']++;
			}
		}
*/
	}
	free(thread_arg);
	free(thread);


}
