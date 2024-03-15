#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int number;
    
    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    
    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size < 2) {
        fprintf(stderr, "This program requires at least 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    if (rank == 0) {
        // If it's the root process (rank 0), initialize the number
        number = 42;
        
        // Send the number to process with rank 1
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);
    } else if (rank == 1) {
        // If it's the process with rank 1, receive the number
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
    
    // Finalize MPI environment
    MPI_Finalize();
    
    return 0;
}
