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
#include "lab3IO.h"

/* Global variables */
int thread_count;

int size;
int* A;


// IO
// int Lab3LoadInput(double ***A, int *size);
// int Lab3SaveOutput(double* x, int size, double time);

// double** CreateMat(int NumRow, int NumCol);
// int DestroyMat(double** A, int NumRow);
// int PrintMat(double** A, int NumRow, int NumCol);
// double* CreateVec(int size);
// int PrintVec(double* b, int size);
// int DestroyVec(double* b);

void report_timing(double start, double end) {
    GET_TIME(end);
    double total_time = (double) end - start;
    printf("It took %f seconds\n", total_time);
}

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    double     t_start = 0.0;
    double     t_end = 0.0;
    long       thread;
    pthread_t* thread_handles;
    
    if (argc != 2) {
        printf("Get your args together c'mon");
        return 1;
    }
    
    thread_count = atoi(argv[1]);
    thread_handles = malloc(thread_count*sizeof(pthread_t));
    printf("Thread Count: %i\n", thread_count);

    //load Matrix
    double** CreateMat(int NumRow, int NumCol);
    Lab3LoadInput(double ***A, int *size);
    
    GET_TIME(t_start);
    
    for (thread = 0; thread < thread_count; thread++) {
        // pthread_create(&thread_handles[thread], NULL, Pth_mat_mat, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; thread++) {
        //pthread_join(thread_handles[thread], NULL);
    }
        
    report_timing(t_start, t_end);
    
    //Output Matrix

    //Free Memory
    
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

void Gaussian() {

}

void Jordan() {

}
