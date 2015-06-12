
void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N, int num_threads) {

	// perform loop fusion to transform this loop and parallelize it with OpenMP
	#pragma omp parallel for
	for (int i = 1; i < N; i++) {
		a[i][N-1] = 2 * b[i][N-1];
		d[i][N-1] = a[i][N-1] * c[i][N-1];
		for (int j = N-2; j > 0; j--) {
			a[i][j] = 2 * b[i][j];
			d[i][j] = a[i][j] * c[i][j];
			c[i][j] = a[i][j] - a[i][j + 2];
		}
	}
/*
	for (int j = 1; j < N; j++) {
		for (int i = 1; i < N; i++) {
			c[i][j - 1] = a[i][j - 1] - a[i][j + 1];
		}
	}
*/
}
