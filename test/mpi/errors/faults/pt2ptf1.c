/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *
 *  (C) 2009 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
#include "mpi.h"
#include <stdio.h>
#include "mpitest.h"

static char MTEST_Descrip[] = "Test err in status return, using truncated \
messages for MPI_Testall";

int main( int argc, char *argv[] )
{
    int wrank, wsize, rank, size, color;
    int i, j, tmp;
    MPI_Comm newcomm;

    MPI_Init( &argc, &argv );

    MPI_Comm_size( MPI_COMM_WORLD, &wsize );
    MPI_Comm_rank( MPI_COMM_WORLD, &wrank );

    /* Color is 0 or 1; 1 will be the processes that "fault" */
    color = (wrank > 0) && (wrank <= wsize/2);
    MPI_Comm_split( MPI_COMM_WORLD, color, wrank, &newcomm );

    MPI_Barrier( MPI_COMM_WORLD );
    if (color) {
	/* Simulate a fault on some processes */
	exit(1);
    }
    
    /* Can we still use newcomm? */
    MPI_Comm_size( newcomm, &size );
    MPI_Comm_rank( newcomm, &rank );

    for (j=0; j<rank; j++) {
	MPI_Recv( &tmp, 1, MPI_INT, j, 0, newcomm, MPI_STATUS_IGNORE );
    }
    for (j=rank+1; j<size; j++) {
	MPI_Send( &rank, 1, MPI_INT, j, 0, newcomm );
    }

    MPI_Comm_free( &newcomm );
    MPI_Finalize();

    printf( " No Errors\n" );

    return 0;
}
