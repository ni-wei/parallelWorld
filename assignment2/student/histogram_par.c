#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "histogram.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int my_counter;
char *buffer_ptr;

struct pthread_args
{
	unsigned int idx;
	unsigned int chunk_size;
	// char *buffer;
	unsigned int* histogram;
	// histogram_t histogram;
};


void * local_histo (void * ptr)

{

	unsigned int i;
	struct pthread_args *arg = ptr;

	// build local histogram


	while (1){

// get chunk
		pthread_mutex_lock(&mutex);


		if(buffer_ptr[my_counter*arg->chunk_size]!=TERMINATOR)
			arg->idx = my_counter++;
		else{
			pthread_mutex_unlock(&mutex);
			break;
		}

		// my_counter++;
		pthread_mutex_unlock(&mutex);

		for (i=arg->idx*arg->chunk_size; i<(arg->idx+1)*arg->chunk_size; i++) {
				if (buffer_ptr[i]==TERMINATOR)
					break;
				else if (buffer_ptr[i] >= 'a' && buffer_ptr[i] <= 'z')
					arg->histogram[buffer_ptr[i]-'a']++;
				else if(buffer_ptr[i] >= 'A' && buffer_ptr[i] <= 'Z')
					arg->histogram[buffer_ptr[i]-'A']++;

		}
	}


	return NULL;
}



void get_histogram(char *buffer,
		   			 unsigned int* histogram,
		   			 unsigned int num_threads,
						 unsigned int chunk_size) {

	// enter your code here

	buffer_ptr = buffer;
	pthread_t *thread; struct pthread_args *thread_arg;
	thread = calloc ( num_threads, sizeof ( *thread ) ) ;
	
	thread_arg = calloc ( num_threads, sizeof ( *thread_arg ) ) ;
	my_counter = 0;

	pthread_mutex_init(&mutex, NULL);


	for ( int k = 0; k < num_threads ; k++){

		thread_arg[k].chunk_size = chunk_size;
		// thread_arg[k].buffer = buffer;
		thread_arg[k].idx = k;
		thread_arg[k].histogram = calloc ( NALPHABET, sizeof (*histogram) ) ;

		pthread_create ( thread + k , NULL, &local_histo, thread_arg + k ) ;




	}


	for ( int k = 0; k < num_threads ; k++){

		pthread_join ( thread [ k ] , NULL ) ;
	}

	for ( int k = 0; k < num_threads ; k++){

		for ( int cnt = 0; cnt < NALPHABET ; cnt++)
			histogram[cnt] += thread_arg[k].histogram[cnt];
	}

/**/
	for ( int k = 0; k < num_threads ; k++){

		free(thread_arg[k].histogram);

	}

	free(thread);
	free(thread_arg);
	pthread_mutex_destroy(&mutex);



}
