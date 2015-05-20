#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"

pthread_mutex_t mutex;
unsigned int counter;
char *buffer_ptr;

struct pthread_args
{
	unsigned int tid;
	// char *buffer;
	unsigned int chunk_size;
	// unsigned int num_threads;
	unsigned int* histogram;
};


void * local_histo (void * ptr)
// void * hello ()
{

	unsigned int i;
	struct pthread_args *arg = ptr;


	// build local histogram
/*
	for (i=arg->tid; i<arg->nBlocks; i+=arg->num_threads) {
		for (j=0; j<BLOCKSIZE; j++) {
	for (i=0; i<arg->nBlocks; i++) {
		for (j=BLOCKSIZE/(arg->num_threads)*(arg->tid); j<BLOCKSIZE/(arg->num_threads)*(arg->tid + 1); j++) {
	for (i=arg->nBlocks/(arg->num_threads)*(arg->tid); i<arg->nBlocks/(arg->num_threads)*(arg->tid + 1); i++) {
		for (j=0; j<BLOCKSIZE; j++) {

*/
		// pthread_mutex_lock(&mutex);

// get chunk
	// for ( counter = 0; buffer[counter*chunk_size]!=TERMINATOR ; counter++){
	while (buffer_ptr[(counter+1)*arg->chunk_size]!=TERMINATOR){

		for (i=counter*arg->chunk_size; i<(counter+1)*arg->chunk_size; i++) {
				if (buffer_ptr[i]==TERMINATOR)
					break;
				if (buffer_ptr[i] >= 'a' && buffer_ptr[i] <= 'z')
					arg->histogram[buffer_ptr[i]-'a']++;
				else if(buffer_ptr[i] >= 'A' && buffer_ptr[i] <= 'Z')
					arg->histogram[buffer_ptr[i]-'A']++;

		}

		pthread_mutex_lock(&mutex);
		counter++;
		pthread_mutex_unlock(&mutex);
/*
		if (buffer[counter*chunk_size]==TERMINATOR)
			break;
*/

	}
		// pthread_mutex_unlock(&mutex);


	// return arg->histogram;

	return NULL;
}



void get_histogram(char *buffer,
		   			 unsigned int* histogram,
		   			 unsigned int num_threads,
						 unsigned int chunk_size) {

	// enter your code here
	counter = 0;
	buffer_ptr = buffer;
	pthread_t * thread; struct pthread_args * thread_arg;
	thread = calloc ( num_threads, sizeof ( * thread ) ) ;
	
	thread_arg = calloc ( num_threads, sizeof ( * thread_arg ) ) ;

	// pthread_mutex_init(&mutex, NULL);

	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("\n mutex init failed\n");
	}


	// while (buffer[counter*chunk_size]!=TERMINATOR){
	for ( int k = 0; k < num_threads ; k++){
		// if (counter < 4)
		// unsigned int k = counter % num_threads;
		thread_arg[k].tid = k;
		thread_arg[k].chunk_size = chunk_size;

		// pthread_mutex_lock(&mutex);
		// thread_arg[k].buffer = buffer;
/*
		for ( int idx = 0; idx < NALPHABET ; idx++)
			thread_arg[k].histogram[idx] = 0;
*/
		pthread_create ( thread + k , NULL, &local_histo, thread_arg + k ) ;
		// pthread_mutex_unlock(&mutex);



	}


	for ( int k = 0; k < num_threads ; k++){

		pthread_join ( thread [ k ] , NULL ) ;
		for ( int idx = 0; idx < NALPHABET ; idx++)
			histogram[idx] += thread_arg[k].histogram[idx];

	}

/*	
	for ( int k = 0; k < num_threads ; k++){
		thread_arg[k].tid = k;
		thread_arg[k].chunk_size = chunk_size;
		thread_arg[k].buffer = buffer;
		// thread_arg[k].num_threads = num_threads;

		for ( int idx = 0; idx < NALPHABET ; idx++)
			thread_arg[k].histogram[idx] = 0;

		pthread_create ( thread + k , NULL, &local_histo, thread_arg + k ) ;
		// pthread_create ( thread + k , NULL, &hello , NULL ) ;
	
	}

	for ( int k = 0; k < num_threads ; k++){

		pthread_join ( thread [ k ] , NULL ) ;
		for ( int idx = 0; idx < NALPHABET ; idx++)
			histogram[idx] += thread_arg[k].histogram[idx];

	}
		pthread_mutex_lock(&mutex);
		pthread_mutex_unlock(&mutex);
*/
	pthread_mutex_destroy(&mutex);

	free(thread);
	free(thread_arg);


}
