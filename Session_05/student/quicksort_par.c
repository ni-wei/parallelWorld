#include <omp.h>

#include "quicksort.h"
#include "helper.h"

//void quicksort(int *a, int left, int right, int num_threads)
void section_parallel_qs(int *a, int left, int right, int num_threads)
{

	// insert your code here
	if(left < right)
	{
		int x = left, y = (left+right)/2, z =right;
		int pivotIdx = (a[x] <= a[y])
		    ? ((a[y] <= a[z]) ? y : ((a[x] < a[z]) ? z : x))
		    : ((a[x] <= a[z]) ? x : ((a[y] < a[z]) ? z : y));

	  int pivotVal = a[pivotIdx];
	  swap(a + pivotIdx, a + right);

	  int swapIdx = left;

	  for(int i=left; i < right; i++)
	  {
		  if(a[i] <= pivotVal)
		  {
			  swap(a + swapIdx, a + i);
			  swapIdx++;
		  }
	  }
	  swap(a + swapIdx, a + right);

//		omp_set_max_active_levels(8);

		if(omp_get_active_level() == 8) {
			section_parallel_qs(a, left, swapIdx - 1, num_threads);
			section_parallel_qs(a, swapIdx + 1, right, num_threads);
		}
		else {
#pragma omp parallel sections
{
#pragma omp section
	  section_parallel_qs(a, left, swapIdx - 1, num_threads);
#pragma omp section
	  section_parallel_qs(a, swapIdx + 1, right, num_threads);
}
		}
	}
}

void quicksort(int *a, int left, int right, int num_threads)
{
	omp_set_nested(1); //nested
	omp_set_num_threads(num_threads);
	omp_set_max_active_levels(8);
	section_parallel_qs(a,left,right,num_threads);
}
