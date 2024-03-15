#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int *array = NULL;
    int array_size = 10;
    int sum = 0, product = 1, subtract = 0;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        fprintf(stderr, "This program requires exactly 4 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Master process
    if (rank == 0) {
        // Allocate memory for the array
        array = (int *)malloc(array_size * sizeof(int));

        // Initialize the array from user input
        printf("Enter %d integers for the array:\n", array_size);
        for (int i = 0; i < array_size; i++) {
            scanf("%d", &array[i]);
        }

        // Send the array to each process
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(array, array_size, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
        
        // Master process calculates the sum
        for (int i = 0; i < array_size; i++) {
            sum += array[i];
        }

        printf("Sum of elements: %d\n", sum);

        // Free the memory allocated for the array
        free(array);
    }
    // Process for addition
    else if (rank == 1) {
        // Receive the array from the master process
        array = (int *)malloc(array_size * sizeof(int));
        MPI_Recv(array, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate the sum of elements
        for (int i = 0; i < array_size; i++) {
            sum += array[i];
        }

        printf("Process %d calculated sum: %d\n", rank, sum);

        // Free the memory allocated for the array
        free(array);
    }
    // Process for multiplication
    else if (rank == 2) {
        // Receive the array from the master process
        array = (int *)malloc(array_size * sizeof(int));
        MPI_Recv(array, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate the product of elements
        for (int i = 0; i < array_size; i++) {
            product *= array[i];
        }

        printf("Process %d calculated product: %d\n", rank, product);

        // Free the memory allocated for the array
        free(array);
    }
    // Process for subtraction
    else if (rank == 3) {
        // Receive the array from the master process
        array = (int *)malloc(array_size * sizeof(int));
        MPI_Recv(array, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate the subtraction of elements
        for (int i = 0; i < array_size; i++) {
            subtract -= array[i];
        }

        printf("Process %d calculated subtraction: %d\n", rank, subtract);

        // Free the memory allocated for the array
        free(array);
    }

    // Finalize MPI environment
    MPI_Finalize();

    return 0
    