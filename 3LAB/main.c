/* ECE 420 - LAB 3
 * 
 * Gaussian-Jordan Elimination Serial Algorithm 
 * Algorithm from serialtester.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "timer.h" // For speed testing
#include "Lab3IO.h"

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
    index = malloc(size * sizeof(int));
    for (i = 0; i < size; ++i)
        index[i] = i;

    if (size == 1)
        X[0] = A[0][1] / A[0][0];
    else{
        //Gaussian
        for (k = 0; k < size - 1; ++k){
            /*Pivoting*/
            temp = 0;
            j = 0;
            for (i = k; i < size; ++i) {
                if (temp < A[index[i]][k] * A[index[i]][k]){
                    temp = A[index[i]][k] * A[index[i]][k];
                    j = i;
                }
            }
            if (j != k)/*swap*/{
                i = index[j];
                index[j] = index[k];
                index[k] = i;
            }
            /*calculating*/
            for (i = k + 1; i < size; ++i){
                temp = A[index[i]][k] / A[index[k]][k];
                for (j = k; j < size + 1; ++j) {
                    A[index[i]][j] -= A[index[k]][j] * temp;
                }
            }       
        }

        //Jordan
        
            for (k = size - 1; k > 0; --k){
                for (i = k - 1; i >= 0; --i ){
                    temp = A[index[i]][k] / A[index[k]][k];
                    A[index[i]][k] -= temp * A[index[k]][k];
                    A[index[i]][size] -= temp * A[index[k]][size];
                } 
            }
        

        // Solve 
        for (k=0; k< size; ++k) {
            X[k] = A[index[k]][size] / A[index[k]][k];
        }

    return;
    }
 }  /* Pth_mat_mat */
