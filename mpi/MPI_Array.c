#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 6

int main(int argc, char **argv)
{
    int rank, size;
    int array[ARRAY_SIZE] = {10, 5, 42, 17, 45, 24};
    int sum = 0, diff = 0, product = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        printf("This program requires exactly 4 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        // Master process initializes the array
        // printf("Enter %d elements of the array:\n", ARRAY_SIZE);
        // for (int i = 0; i < ARRAY_SIZE; i++)
        // {
        //     scanf("%d", &array[i]);
        // }

        // Send the array to other processes
        for (int i = 1; i < size; i++)
        {
            MPI_Send(array, ARRAY_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        // Other processes receive the array
        MPI_Recv(array, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Perform different operations based on rank
        if (rank == 1)
        {
            // Calculate sum
            for (int i = 0; i < ARRAY_SIZE; i++)
            {
                sum += array[i];
            }
            printf("Sum calculated by process %d: %d\n", rank, sum);
        }
        else if (rank == 2)
        {
            // Calculate difference
            for (int i = 0; i < ARRAY_SIZE; i++)
            {
                diff -= array[i];
            }
            printf("Difference calculated by process %d: %d\n", rank, diff);
        }
        else if (rank == 3)
        {
            // Calculate product
            for (int i = 0; i < ARRAY_SIZE; i++)
            {
                product *= array[i];
            }
            printf("Product calculated by process %d: %d\n", rank, product);
        }
    }

    MPI_Finalize();
    return 0;
}
