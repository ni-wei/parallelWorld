#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "helper.h"

void reverse(char *str, int strlen)
{
	// parallelize this function and make sure to call reverse_str()
	// on each processor to reverse the substring.

	// Three steps:
	// 1) Distribute array from rank 0 to all ranks using MPI_Scatterv()
	// 2) Call provided reverse function on the local part of the array
	// 3) Send local part of the array back to Rank 0 and store it
	//    directly at the right position.
	/*
	Hints:	Implement scatterv first and make sure that it’s working correctly!
		You can use the provided print function to print the char buffer.
	*/
	
	int np, rank;

	int *sendcounts;    //how many elements to send to each process
	int *displs;        //the displacements where each segment begins

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int rem = strlen % np; // elements remaining after division among processes
	int sum = 0;                // Sum of counts. Used to calculate displacements
	char rec_buf[100];          // buffer where the received data should be stored
//	char *rec_buf;          // buffer where the received data should be stored

	sendcounts = malloc(np * sizeof(int)); // or calloc?
	displs = malloc(np * sizeof(int));
	// calculate send counts and displacements
	for (int i = 0; i < np; i++) {
		sendcounts[i] = strlen / np;
		if (rem > 0) {
			sendcounts[i]++;
			rem--;
		}

		displs[i] = sum;
		sum += sendcounts[i];
	}

//	rec_buf = malloc(sendcounts[rank] * sizeof(char));

	// print calculated send counts and displacements for each process
	if (0 == rank) {
		for (int i = 0; i < np; i++) {
			printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
		}
	}

		MPI_Scatterv(str, sendcounts, displs, MPI_CHAR, &rec_buf, 100, MPI_CHAR, 0, MPI_COMM_WORLD);

    // print what each process received
    printf("%d: ", rank);
    for (int i = 0; i < sendcounts[rank]; i++) {
        printf("%c...", rec_buf[i]);
    }
    printf("\n");

    if (rank == 0)
    {
    	reverse_str(str, strlen);
    }

	//reverse_str(&rec_buf, sendcounts[rank]);

	free(sendcounts);
	free(displs);
//	free(rec_buf);
}
