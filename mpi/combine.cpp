#include <iostream>
#include <sstream>
#include<fstream>
#include<NTL/matrix.h>
#include <NTL/mat_ZZ.h>
#include <NTL/ZZ_p.h>
#include <mpi.h>

using namespace std;
using namespace NTL;


/// @brief 
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size<2)
    {
        cerr<<"This program requires atleast 2 processes."<<endl;
        MPI_Abort(MPI_COMM_WORLD,1);

    }
      ifstream file("INPUT1.txt");
    // Master process takes the input matrix from the user
   // Mat<ZZ> matrix;
   // matrix.SetDims(3,3);
    if (rank == 0) {
        Mat<ZZ> matrix;
        matrix.SetDims(3,3);
        file>>matrix;
        cout<<"\nrows::"<<matrix.NumRows()<<endl;
     
        stringstream ss;
        ss<<matrix;
        string s=ss.str();
        int strLen=s.length()+1;
        char *ker_str=new char[strLen];
        strcpy(ker_str,s.c_str());

        MPI_Send(&strLen,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(ker_str,strLen,MPI_CHAR,1,0,MPI_COMM_WORLD);
        ss.clear();
        delete ker_str;
        
        
        
    }

    else
    {
     //Receive the matrix from process 0

        int strLen;
        MPI_Recv(&strLen,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        char*strKer=new char[strLen];
        MPI_Recv(strKer,strLen,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        Mat<ZZ>matrix;
        matrix.SetDims(3,3);
        stringstream ss;
        ss<<strKer;
        ss>>matrix;
        delete strKer;
        ss.clear();

        cout<<"cols::"<<matrix.NumCols()<<endl;

        cout<<"matrix received!"<<endl;
        cout<<"Determinant is:"<<determinant(matrix)<<endl;

    }
        

    MPI_Finalize();
    file.close();
    return 0;
    
}