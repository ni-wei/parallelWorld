
void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N, int num_threads) {

	// perform loop fission to transform this loop and parallelize it with OpenMP
	#pragma omp parallel
	{ //for schedule(runtime) private(j)
		for (int i = 1; i < N; i++) {
			#pragma omp for nowait//schedule(static)
			for (int j = 1; j < N; j++) {
				c[i][j] = 3 * d[i][j];
				d[i][j]   = 2 * c[i + 1][j];
			}
		}
		#pragma omp barrier
		for (int j = 1; j < N; j++) {
			#pragma omp for nowait//schedule(static)
			for (int i = 1; i < N; i++) {
				a[i][j]   = a[i][j] * b[i][j];
				b[i][j + 1] = 2 * a[i][j] * c[i - 1][j];
			}
		}
	}
}
