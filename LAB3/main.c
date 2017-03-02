/* File:
 *     main.c
 *
 * Purpose:
 *
 * Input:
 *     p: number of threads
 *
 * Output:
 *
 * Usage:
 *     main <thread_count>
 *
 * Notes:
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "timer.h" // For speed testing
#include "Lab3IO.h"


void Gaussian(double** A, int n);
void Jordan();
void CopyMatrix(double **A, double** B, int rows, int cols);
void SwapRow(double **A, int row1, int row2, int col_size);

/* Global variables */
int thread_count;

int size;
double** A;

double *x;

// IO
// int Lab3LoadInput(double ***A, int *size);
// int Lab3SaveOutput(double* x, int size, double time);

// double** CreateMat(int NumRow, int NumCol);
// int DestroyMat(double** A, int NumRow);
// int PrintMat(double** A, int NumRow, int NumCol);
// double* CreateVec(int size);
// int PrintVec(double* b, int size);
// int DestroyVec(double* b);

double report_timing(double start, double end) {
    GET_TIME(end);
    double total_time = (double) end - start;
    printf("It took %f seconds\n", total_time);
    return total_time;
}




/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    double     t_start = 0.0;
    double     t_end = 0.0;
    long       thread;
    //pthread_t* thread_handles;
    
    if (argc != 2) {
        printf("Get your args together c'mon");
        return 1;
    }
    
    thread_count = atoi(argv[1]);
    //thread_handles = malloc(thread_count*sizeof(pthread_t));
    printf("Thread Count: %i\n", thread_count);

    //load Matrix
    A = CreateMat(size, size);
    x = CreateVec(size);
    Lab3LoadInput(&A, &size);

    PrintMat(A, size, size);
    
    GET_TIME(t_start);
    
    for (thread = 0; thread < thread_count; thread++) {
        // pthread_create(&thread_handles[thread], NULL, Pth_mat_mat, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; thread++) {
        //pthread_join(thread_handles[thread], NULL);
    }
        
    double t = report_timing(t_start, t_end);
    
    Gaussian(A, size);
    PrintMat(A, size, size);

    //Output Matrix
    Lab3SaveOutput(x, size, t);

    //Free Memory
    DestroyVec(x);
    DestroyMat(A, size);
    
    return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:       Solve
 * Purpose:        Solve using Partial Pivot
 * In arg:         
 * Global in vars: 
 * Global out var: 
 */
 void *solve(void* rank) {
     
    //Gaussian

    //Barrier?

    //Jordan 

    return NULL;
 }  /* Pth_mat_mat */



void Gaussian(double** A, int n) {
    int largest_index = 0;
    int largest_value = 0;

    for (int k = 0; k < n; k++) {
        /*Pivoting*/
        //Search for the row which has the max value in column k.
        for (int i = k; i < n; i++) {
            if (A[i][k] > largest_value) {
                largest_value = A[i][k];
                largest_index = i;
            }
        }
        // Swap the current row with the row that has the maximum value.
        SwapRow(A, k, largest_index, n);

        for (int i = k+1; i < n; i++) {
            double temp = A[i][k]/A[k][k];
            for (int j = k; j < n; j++) {
                A[i][j] = A[i][j] - (temp*A[k][j]);
            }
        }
    }
}

void Jordan() {

}


void CopyMatrix(double **A, double** B, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            B[i][j] = A[i][j];
        }
    }
}

void SwapRow(double **A, int row1, int row2, int col_size) {
    for (int j = 0; j < col_size; j++) {
        double tmp = A[row1][j];
        A[row1][j] = A[row2][j];
        A[row2][j] = tmp;
    }
}
