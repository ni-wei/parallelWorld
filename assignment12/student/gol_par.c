#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include "helper.h"

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps)
{
	// READ ME! Parallelize this function to work with MPI. It must work even with a single processor.
	// We expect you to use MPI_Scatterv, MPI_Gatherv, and MPI_Sendrecv to achieve this.
	// MPI_Scatterv/Gatherv are checked to equal np times, and MPI_Sendrecv is expected to equal 2 * np * timesteps
	// That is, top+bottom ghost cells * all processors must execute this command * Sendrecv executed every timestep.

	unsigned char *grid_in, *grid_out, *grid_tmp;
//	size_t size = sizeof(unsigned char) * dim_x * dim_y;

	int np, rank;
	int next, prev;

	int *sendcounts;    //how many rows to send to each process
	int *displs;        //the displacements where each segment begins
	unsigned int *sub_dim_x;

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int rem = dim_x % np;	// elements remaining after division among processes
	int sum = 0;		// Sum of send counts. Used to calculate send displacements
	//unsigned char rec_buf[100];	// buffer where the received data should be stored
	unsigned char *rec_buf;		// buffer where the received data should be stored
	sendcounts = malloc(np * sizeof(int)); // or calloc?
	displs = malloc(np * sizeof(int));
	sub_dim_x = malloc(np * sizeof(unsigned int));

	// calculate send counts and displacements
	for (int i = 0; i < np; i++) {
		sendcounts[i] = dim_x / np;
		if (rem > 0) {
			sendcounts[i]++;
			rem--;
		}
		sub_dim_x[i] = sendcounts[i] + 2;
		sendcounts[i] *= dim_y;
		displs[i] = sum;
		sum += sendcounts[i];
	}
	unsigned int sub_size = sub_dim_x[rank] *dim_y;
//	int sub_size = sendcounts[rank];
	rec_buf = calloc(sizeof(unsigned char), sub_size);

	// print calculated send counts and displacements for each process
	if (0 == rank) {
		for (int i = 0; i < np; i++) {
			printf("sendcounts[%d] = %d\tdispls[%d] = %d\tsub_dim_x[%d] = %d\n", i, sendcounts[i], i, displs[i], i, sub_dim_x[i]);
		}
	}

	MPI_Scatterv(grid, sendcounts, displs, MPI_CHAR, rec_buf + dim_y, sendcounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

	grid_tmp = calloc(sizeof(unsigned char), sub_size);
	if (grid_tmp == NULL)
		exit(EXIT_FAILURE);
	grid_in = rec_buf;
	grid_out = grid_tmp;

	/* Set neighbors */
	if (rank == 0)
		prev = np-1;
	else
		prev = rank-1;
	if (rank == np - 1)
		next = 0;
	else
		next = rank+1;
/*
	grid_tmp = calloc(sizeof(unsigned char), dim_y * dim_x);
	if (grid_tmp == NULL)
		exit(EXIT_FAILURE);

	grid_in = grid;
	grid_out = grid_tmp;
*/
//if(0 == rank){
	for (int t = 0; t < time_steps; ++t)
	{
		MPI_Sendrecv(rec_buf + dim_y, dim_y, MPI_CHAR, prev, 1,
			rec_buf, dim_y, MPI_CHAR, prev, 0,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Sendrecv(rec_buf + sub_size - 2*dim_y, dim_y, MPI_CHAR, next, 0,
			rec_buf + sub_size - dim_y, dim_y, MPI_CHAR, next, 1,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int y = 0; y < dim_y; ++y)
		{
			for (int x = 0; x < sub_dim_x[rank]; ++x)
			{
				evolve(grid_in, grid_out, sub_dim_x[rank], dim_y, x, y);
			}
		}
		swap((void**)&grid_in, (void**)&grid_out);
	}
//}
/*
	if (grid != grid_in)
		memcpy(grid, grid_in, size);
*/
	if (rec_buf != grid_in)
		memcpy(rec_buf, grid_in, sizeof(unsigned char) * sub_size);

	free(grid_tmp);
	MPI_Gatherv(rec_buf, sendcounts[rank], MPI_CHAR, grid, sendcounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);

	return cells_alive(grid, dim_x, dim_y);

	free(sendcounts);
	free(displs);
	free(sub_dim_x);
//	free(rec_buf);
}
