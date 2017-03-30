/* ECE 420 - LAB 3
 * 
 * Gaussian-Jordan Elimination OMP Algorithm
 * Algorithm from serialtester.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "timer.h" // For speed testing
#include "Lab3IO.h"
#include <omp.h>

void Solve(double** A, double* X, int size);

/* Global variables */
int thread_count;

int size;
double** A;
double* X;
int* index;

double report_timing(double start, double end) {
    GET_TIME(end);
    double total_time = (double) end - start;
    printf("It took %f seconds\n", total_time);
    return total_time;
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    double     t_start = 0.0;
    double     t_end = 0.0;
    
    if (argc != 2) {
        printf("Get your args together c'mon");
        return 1;
    }
    
    thread_count = atoi(argv[1]);
    printf("Thread Count: %i\n", thread_count);

    //load Matrix
    Lab3LoadInput(&A, &size);
    X = CreateVec(size);
    

    //PrintMat(A, size, size);
    
    GET_TIME(t_start);

    Solve(A, X, size);

    double t = report_timing(t_start, t_end);
    
    // printf("Final A: \n");
    // PrintMat(A, size, size);
    // printf("Final X: \n");
    // PrintVec(X, size);

    //Output Matrix
    Lab3SaveOutput(X, size, t);

    //Free Memory
    DestroyVec(X);
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
 void Solve(double** A, double* X, int size) {
    int i, j, k;
    double temp;
    // double timer;
    // double timer2;
    index = malloc(size * sizeof(int));

    omp_set_num_threads(thread_count);
    printf("omp set to %d threads\n", thread_count);


    // timer = omp_get_wtime();
    // # pragma omp parallel for
    for (i = 0; i < size; ++i)
        index[i] = i;
    // printf("index init took: %f time\n", omp_get_wtime()-timer);

    if (size == 1)
        X[0] = A[0][1] / A[0][0];
    else{

        //### Gaussian
        // This part of the algorithm prooves tom be the most time consuming.
        // timer = omp_get_wtime();
        {
            for (k = 0; k < size - 1; ++k){
                /*Pivoting*/
                temp = 0;
                j = 0;

                // timer2 = omp_get_wtime();
                for (i = k; i < size; ++i) {
                    if (temp < A[index[i]][k] * A[index[i]][k]){
                        temp = A[index[i]][k] * A[index[i]][k];
                        j = i;
                    }
                }
                // printf("Pivot took: %f time\n", omp_get_wtime()-timer2);

                if (j != k)/*swap*/{
                    i = index[j];
                    index[j] = index[k];
                    index[k] = i;
                }


                /*calculating*/
                // timer2 = omp_get_wtime();
                // This double nested for loop is the most consuming block of code
                // this is where parrallelization will make the biggest  boost in 
                // efficiency
                #pragma omp parallel for shared(A) private(temp, i, j)
                for (i = k + 1; i < size; ++i){
                    temp = A[index[i]][k] / A[index[k]][k];
                    for (j = k; j < size + 1; ++j) {
                        A[index[i]][j] -= A[index[k]][j] * temp;
                    }
                }      
                // printf("Calc took: %f time\n", omp_get_wtime()-timer2);
            }
        }
        // printf("\nGaussian took: %f time\n", omp_get_wtime()-timer);

        //### Jordan
        // This section needs to be executed sequentially
        // timer = omp_get_wtime();
        for (k = size - 1; k > 0; --k){
            for (i = k - 1; i >= 0; --i ){
                temp = A[index[i]][k] / A[index[k]][k];
                A[index[i]][k] -= temp * A[index[k]][k];
                A[index[i]][size] -= temp * A[index[k]][size];
            } 
        }
        // printf("Jordan took: %f time\n", omp_get_wtime()-timer);


        //### Solve 

        // timer = omp_get_wtime();
        for (k=0; k< size; ++k) {
            X[k] = A[index[k]][size] / A[index[k]][k];
        }
        // printf("Solve took: %f time\n", omp_get_wtime()-timer);


    return;
    }
 }  /* Pth_mat_mat */
