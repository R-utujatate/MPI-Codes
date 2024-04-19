#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <mpi.h>
#include <NTL/mat_ZZ.h>

using namespace std;
using namespace NTL;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        cerr << "This program must be run with 4 processes (1 master, 3 slaves)." << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) { // Master process
        ifstream inputFile("matrix.txt");
        if (!inputFile) {
            cerr << "Failed to open input file." << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        stringstream ss;
        ss << inputFile.rdbuf();
        string matrixStr = ss.str();

        // Broadcast the matrix to all slave processes
        int matrixSize = matrixStr.size() + 1; // Include null terminator
        MPI_Bcast(&matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD); // Broadcast the size first

        // Broadcast the matrix string
        char* matrixBuffer = new char[matrixSize];
        strcpy(matrixBuffer, matrixStr.c_str());
        MPI_Bcast(matrixBuffer, matrixSize, MPI_CHAR, 0, MPI_COMM_WORLD);

        // Calculate determinant of the matrix in master process
        mat_ZZ mat;
        ss.clear();
        ss.str(matrixStr);
        ss >> mat;
        //ZZ determinant = det(mat);

        //cout << "Determinant calculated by master process: " << determinant(mat) << endl;

        delete[] matrixBuffer; // Free dynamically allocated memory
    } else { // Slave processes
        // Receive the size of the matrix string
        int matrixSize;
        MPI_Bcast(&matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Receive the matrix string
        char* matrixBuffer = new char[matrixSize];
        MPI_Bcast(matrixBuffer, matrixSize, MPI_CHAR, 0, MPI_COMM_WORLD);

        stringstream ss;
        ss << matrixBuffer;

        // Convert string to matrix
        mat_ZZ mat;
        ss >> mat;

        // Calculate determinant in each slave process
        //ZZ determinant = det(mat);

        cout << "Determinant calculated by slave process"<<rank<<": "<< determinant(mat)<<endl;

        delete[] matrixBuffer; // Free dynamically allocated memory
    }

    MPI_Finalize();
    return 0;
}
