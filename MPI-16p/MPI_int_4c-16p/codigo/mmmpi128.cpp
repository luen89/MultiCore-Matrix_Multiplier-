#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include "mpi.h"
#include <unistd.h>
#include <ios>
#include <string>

#define MATRIX_SIZE 128

int first_matrix[MATRIX_SIZE][MATRIX_SIZE];
int second_matrix[MATRIX_SIZE][MATRIX_SIZE];

void process_mem_usage(double& vm_usage, double& resident_set){
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

typedef struct {
    int proc_count;
    int dim;
    int row;
    int col;
    int rank;
    MPI_Comm grid_comm;
    MPI_Comm row_comm;
    MPI_Comm col_comm;
} GridStructure;

void GridSetup(GridStructure *grid) {
    int dimensions[2];
    int wrap_around[2];
    int coordinates[2];
    int free_coords[2];
    int world_rank;
    
    
    MPI_Comm_size(MPI_COMM_WORLD, &(grid->proc_count));
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    grid->dim = (int) sqrt((double) grid->proc_count);
    dimensions[0] = dimensions[1] = grid->dim;

    wrap_around[0] = 0;
    wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->grid_comm));
    MPI_Comm_rank(grid->grid_comm, &(grid->rank));
    MPI_Cart_coords(grid->grid_comm, grid->rank, 2, coordinates);
    grid->row = coordinates[0];
    grid->col = coordinates[1];

    free_coords[0] = 0;
    free_coords[1] = 1;
    MPI_Cart_sub(grid->grid_comm, free_coords, &(grid->row_comm));

    free_coords[0] = 1;
    free_coords[1] = 0;
    MPI_Cart_sub(grid->grid_comm, free_coords, &(grid->col_comm));
}
void MultiplyLocal(int **a, int **b, int **c, int size) {
    int temp = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            temp = 0;
            for (int k = 0; k < size; k++)
                temp += (a[i][k] * b[k][j]);
            c[i][j] += temp;
        }
}
void UnpackMatrix(int *buff, int **a, int size) {
    int k = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            a[i][j] = buff[k];
            k++;
        }
}
void PackMatrix(int *buff, int **a, int size) {
    int k = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            buff[k] = a[i][j];
            k++;
        }
}
void PrintMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << "|";
        for (int j = 0; j < size; j++) {
            int el = matrix[i][j];
            if (el < 10)
                std::cout << " ";
            std::cout << el;
            std::cout << "|";
        }
        std::cout << std::endl;
    }
}
void PrintPackedMatrix(int *matrix, int size) {
    for (int i = 0; i < size; i++) {
       // std::cout << "|";
        for (int j = 0; j < size; j++) {
            int el = matrix[i * size + j];
            if (el < 10)
                std::cout << " ";
            std::cout << el;
            std::cout << "|";
        }
        std::cout << std::endl;
    }
}

//rand() % (upper - lower + 1)) + lower;
void GenerateMatrices() {
	srand(time(NULL));
    for (int i = 0; i < MATRIX_SIZE; i++){
      //std::cout << "\n";
    	
      	for (int j = 0; j < MATRIX_SIZE; j++) {
      		
            first_matrix[i][j] = (int)(1000 + rand() % (2000-1000+1));//(i==j) ? 1 : 0;
            //std::cout << first_matrix[i][j];
            //std::cout << "__";
            second_matrix[i][j] = (int)(1000 + rand() % (2000-1000+1));
            //std::cout << second_matrix[i][j];
            //std::cout << "//";
        }
    }
   	//std::cout << std::endl;
}

void FoxMultiply(int n, GridStructure *grid, int **a, int **b, int **c) {
    int **temp_a, *buff, stage, root, submat_dim, src, dst;
    MPI_Status status;

    submat_dim = n / grid->dim;

    temp_a = new int*[submat_dim];
    for(int i = 0; i < submat_dim; ++i)
        temp_a[i] = new int[submat_dim];
    for (int i = 0; i < submat_dim; i++)
        for (int j = 0; j < submat_dim; j++)
            temp_a[i][j] = 0;

    buff = new int[submat_dim*submat_dim];
    for (int i = 0; i < submat_dim * submat_dim; i++)
        buff[i] = 0;

    src = (grid->row + 1) % grid->dim;
    dst = (grid->row + grid->dim - 1) % grid->dim;

    for (stage = 0; stage < grid->dim; stage++) {
        root = (grid->row + stage) % grid->dim;
        if (root == grid->col) {
            PackMatrix(buff, a, submat_dim);
            MPI_Bcast(buff, submat_dim * submat_dim, MPI_INT, root, grid->row_comm);
            UnpackMatrix(buff, a, submat_dim);
            MultiplyLocal(a, b, c, submat_dim);
        } else {
            PackMatrix(buff, temp_a, submat_dim);
            MPI_Bcast(buff, submat_dim * submat_dim, MPI_INT, root, grid->row_comm);
            UnpackMatrix(buff, temp_a, submat_dim);
            MultiplyLocal(temp_a, b, c, submat_dim);
        }
        PackMatrix(buff, b, submat_dim);
        MPI_Sendrecv_replace(buff, submat_dim * submat_dim, MPI_INT, dst, 0, src, 0, grid->col_comm, &status);
        UnpackMatrix(buff, b, submat_dim);
    }
}

int** TestSingleThread()
{
    int **test_result, **test_a, **test_b;
    test_a = new int*[MATRIX_SIZE];
    test_b = new int*[MATRIX_SIZE];
    test_result = new int*[MATRIX_SIZE];
    for(int i = 0; i < MATRIX_SIZE; ++i) {
        test_a[i] = new int[MATRIX_SIZE];
        test_b[i] = new int[MATRIX_SIZE];
        test_result[i] = new int[MATRIX_SIZE];
    }
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++) {
            test_a[i][j] = first_matrix[i][j];
            test_b[i][j] = second_matrix[i][j];
            test_result[i][j] = 0;
        }
    /*std::cout << "Matriz a:" << std::endl;
    PrintMatrix(test_a, MATRIX_SIZE);
    std::cout << "Matriz b:" << std::endl;
    PrintMatrix(test_b, MATRIX_SIZE);
    */
    MultiplyLocal(test_a, test_b, test_result, MATRIX_SIZE);

    std::cout << "Local result:" << std::endl;
    //PrintMatrix(test_result, MATRIX_SIZE);
    return test_result;
}


int main(int argc, char *argv[]) {
	//int MATRIX_SIZE=argv[1];

	/*
    std::ofstream procesos;
    procesos.open ("pps.txt");
    procesos << getpid() <<std::endl;
    std::cout << getpid() <<std::endl;
    procesos.close();
    */

    //sleep(1);
    int block_size;
    int **local_a, **local_b, **local_c;
    clock_t start,finish;
    MPI_Init(&argc, &argv);

    GridStructure grid;
    GridSetup(&grid);
    GenerateMatrices();

    block_size = MATRIX_SIZE / grid.dim;
    int base_row = grid.row * block_size;
    int base_col = grid.col * block_size;

    local_a = new int*[MATRIX_SIZE];
    local_b = new int*[MATRIX_SIZE];
    local_c = new int*[MATRIX_SIZE];
    for(int i = 0; i < MATRIX_SIZE; ++i)
    {
        local_a[i] = new int[MATRIX_SIZE];
        local_b[i] = new int[MATRIX_SIZE];
        local_c[i] = new int[MATRIX_SIZE];
    }
    for (int i = base_row; i < base_row + block_size; i++)
        for (int j = base_col; j < base_col + block_size; j++) {
            local_a[i - (base_row)][j - (base_col)] = first_matrix[i][j];
            local_b[i - (base_row)][j - (base_col)] = second_matrix[i][j];
            local_c[i - (base_row)][j - (base_col)] = 0;
        }

    MPI_Barrier(grid.grid_comm);
    if (grid.rank == 0)
    {
        start = clock();
    }
    FoxMultiply(MATRIX_SIZE, &grid, local_a, local_b, local_c);
    MPI_Barrier(grid.grid_comm);
    if (grid.rank == 0)
    {
        finish = clock();
        clock_t result_time = finish - start;
        double time_fox=double(result_time)/CLOCKS_PER_SEC;
        //std::cout << "Time: " << time_fox << std::endl;

        FILE *fT, *fM;
        char nomT[35];
        char nomM[35];
        double vm, rss;

        process_mem_usage(vm, rss);
		snprintf(nomT, sizeof(nomT), "archivos/tiempo/TMPre%d.txt",MATRIX_SIZE);
		snprintf(nomM, sizeof(nomM), "archivos/memoria/MMPre%d.txt",MATRIX_SIZE);
		fT=fopen(nomT,"a");
		fM=fopen(nomM,"a");
		fprintf(fT,"%f\n",time_fox );
		fprintf(fM,"%f\n",rss );
  		fclose(fT);
  		fclose(fM);
    }

    int *result_buff = new int[MATRIX_SIZE * MATRIX_SIZE];
    int *local_buff = new int[block_size * block_size];
    PackMatrix(local_buff, local_c, block_size);

    MPI_Gather(local_buff, block_size * block_size, MPI_INT, result_buff, block_size * block_size, MPI_INT, 0, grid.grid_comm);
    MPI_Barrier(grid.grid_comm);
    if (grid.rank == 0) {
        int *data = new int[MATRIX_SIZE * MATRIX_SIZE];
        int k = 0;
        for (int bi = 0; bi < grid.dim; bi++)
            for (int bj = 0; bj < grid.dim; bj++)
                for (int i = bi * block_size; i < bi * block_size + block_size; i++)
                    for (int j = bj * block_size; j < bj * block_size + block_size; j++) {
                        data[i * MATRIX_SIZE + j] = result_buff[k];
                        k++;
                    }

        /*
        std::cout << "Fox result check:";
        //PrintPackedMatrix(data, MATRIX_SIZE);
        
        //clock_t Serialstart,Serialfinish;
        
        //Serialstart = clock();
        int ** res = TestSingleThread();
        //Serialfinish = clock();
        /*
        clock_t Serialresult_time = Serialfinish - Serialstart;
        std::cout << "Normal Time: " << double(Serialresult_time)/CLOCKS_PER_SEC << std::endl;
        
        for(int i=0;i<MATRIX_SIZE;i++)
            for(int j=0;j<MATRIX_SIZE;j++)
            {
                if(res[i][j]!=data[i*MATRIX_SIZE+j])
                {
                    std::cout << "ERROR!" <<std::endl;
                    exit(1);
                }
            }
          
        std::cout << "OK" << std::endl;
        */
    }

    MPI_Finalize();
    exit(0);
}       
