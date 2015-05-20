#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv) {

		#pragma omp parallel
		{
			printf("Hello World from thread %d\n", omp_get_thread_num());
			// only executed by main thread
			if (omp_get_thread_num() == 0)
			printf("Number of threads is %d\n", omp_get_num_threads());
		}
	return 0;
}