#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size, number, cube;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        if (rank == 0)
            printf("This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        // Process 0 sends a number to process 1
        number = 8;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number: %d\n", number);
        // Process 0 receives cubes number from process 1
        MPI_Recv(&cube, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received cube of given number: %d\n", cube);
    }
    else if (rank == 1)
    {
        // Process 1 receives number from process 0
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number: %d\n", number);
        // Process 1 cubes the received number
        cube = number * number * number;
        // Process 1 sends cubes number to process 0
        MPI_Send(&cube, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
