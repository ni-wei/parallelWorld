#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long counter = 0;
// char *buffer_ptr;

struct pthread_args
{

	unsigned int chunk_size;
	char *buffer;
	unsigned int* histogram;
};


void * local_histo (void * ptr)

{

	unsigned int i;
	struct pthread_args *arg = ptr;


	// build local histogram


	while (arg->buffer[(counter+1)*arg->chunk_size]!=TERMINATOR){

		for (i=counter*arg->chunk_size; i<(counter+1)*arg->chunk_size; i++) {
				if (arg->buffer[i]==TERMINATOR)
					break;
				if (arg->buffer[i] >= 'a' && arg->buffer[i] <= 'z')
					arg->histogram[arg->buffer[i]-'a']++;
				else if(arg->buffer[i] >= 'A' && arg->buffer[i] <= 'Z')
					arg->histogram[arg->buffer[i]-'A']++;

		}
// get chunk
		pthread_mutex_lock(&mutex);
		counter++;
		pthread_mutex_unlock(&mutex);


	}


	return NULL;
}



void get_histogram(char *buffer,
		   			 unsigned int* histogram,
		   			 unsigned int num_threads,
						 unsigned int chunk_size) {

	// enter your code here

	buffer_ptr = buffer;
	pthread_t * thread; struct pthread_args * thread_arg;
	thread = calloc ( num_threads, sizeof ( * thread ) ) ;
	
	thread_arg = calloc ( num_threads, sizeof ( * thread_arg ) ) ;

	// pthread_mutex_init(&mutex, NULL);

	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("\n mutex init failed\n");
	}


	for ( int k = 0; k < num_threads ; k++){

		thread_arg[k].chunk_size = chunk_size;


		thread_arg[k].histogram = calloc ( NALPHABET, sizeof ( unsigned int) ) ;

		pthread_create ( thread + k , NULL, &local_histo, thread_arg + k ) ;




	}


	for ( int k = 0; k < num_threads ; k++){

		pthread_join ( thread [ k ] , NULL ) ;
		for ( int idx = 0; idx < NALPHABET ; idx++)
			histogram[idx] += thread_arg[k].histogram[idx];
		free(thread_arg[k].histogram);

	}

	pthread_mutex_destroy(&mutex);

	free(thread);
	free(thread_arg);


}
