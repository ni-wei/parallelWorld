
void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N, int num_threads) {

	// perform loop alignment to transform this loop and parallelize it with OpenMP
	int i,j;

#pragma omp parallel
{
	#pragma omp for private(i)
	for (i = 1; i < N; i++) {
		a[i][1] = 3 * b[i][1];
		c[i][0] = a[i][1] * d[i][1];
	}

	for (i = 1; i < N; i++) {
		#pragma omp for private(j)
		for (j = 2; j < N; j++) {
			a[i][j] = 3 * b[i][j];
			b[i][j] = c[i][j - 1] * c[i][j - 1];
			c[i][j - 1] = a[i][j] * d[i][j];
		}
	}


	#pragma omp for private(i)
	for (i = 1; i < N; i++) {
		b[i][N] = c[i][N-1] * c[i][N-1];
	}
}

}
