/* File:
 *     pth_mat_vect.c
 *
 * Purpose:
 *     Computes a parallel matrix-vector product.  Matrix
 *     is distributed by block rows.  Vectors are distributed by
 *     blocks.
 *
 * Input:
 *     m, n: order of matrix
 *     A, x: the matrix and the vector to be multiplied
 *
 * Output:
 *     y: the product vector
 *
 * Compile:  gcc -g -Wall -o pth_mat_vect pth_mat_vect.c -lpthread
 * Usage:
 *     pth_mat_vect <thread_count>
 *
 * Notes:
 *     1.  Local storage for A, x, y is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both
 *         m and n.  The program doesn't check for this.
 *     3.  We use a 1-dimensional array for A and compute subscripts
 *         using the formula A[i][j] = A[i*n + j]
 *     4.  Distribution of A, x, and y is logical:  all three are
 *         globally shared.
 *
 * IPP:    Section 4.3 (pp. 159 and ff.).  Also Section 4.10 (pp. 191 and
 *         ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "timer.h" // For speed testing
#include "lab1_IO.h"

/* Global variables */
int     thread_count;
int     n;

int* A;
int* B;
int* C;

/* Serial functions */
void Usage(char* prog_name);
void Read_matrix(char* prompt, int **A, int m, int n);
void Print_matrix(char* title, int **A, int m, int n);

/* Parallel function */
void *Pth_mat_mat(void* rank);



void Initialize_matrix(int ***M, int *n)
{
    *M = malloc(*n * sizeof(int*));
    int i;
    for (i = 0; i <= *n; i++)
    {
        (*M)[i] = calloc(*n, sizeof(int));
    }
}



int Get_matrix_value(int **M, int i, int j)
{
    return M[i][j];
}

void Set_matrix_value(int ***M, int i, int j, int value)
{
    (*M)[i][j] = value;
}

// Added functions
bool Is_square(int num){
    return ((int) sqrt(num))*((int) sqrt(num)) == num;
}

bool Is_divisor(int num, int numerator){
    return (numerator % num == 0);
}

/*------------------------------------------------------------------
 * Function:  Find_largest_square_divisor
 * Purpose:   returns the largest possible divisor that is square for the int requested_thread_count
 * In arg :   prog_name
 */
int Find_largest_square_divisor (int requested_thread_count, int matrix_dim){
    int x = requested_thread_count;
    while(x>0){
        if (Is_divisor(x, matrix_dim) && Is_square(x))
            return x;
        --x;
    }
    return 0;
}

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    double     t_start;
    double     t_end;
    long       thread;
    pthread_t* thread_handles;
    
    if (argc != 2) Usage(argv[0]);
    
    Lab1_loadinput(&A, &B, &n);
    
    Initialize_matrix(&C, &n);

    //Print_matrix("Matrix A", A, n, n);
    //Print_matrix("Matrix B", B, n, n);
    //Print_matrix("Matrix C", C, n, n);
    
    thread_count = Find_largest_square_divisor(atoi(argv[1]), n);
    thread_handles = malloc(thread_count*sizeof(pthread_t));
    
    printf("Thread Count: %i\n", thread_count);
    
    GET_TIME(t_start);
    
    for (thread = 0; thread < thread_count; thread++)
         pthread_create(&thread_handles[thread], NULL,
           Pth_mat_mat, (void*) thread);
    
    for (thread = 0; thread < thread_count; thread++)
         pthread_join(thread_handles[thread], NULL);
    
    GET_TIME(t_end);
    double total_time = (double) t_end - t_start;
    
    printf("It took %f seconds\n", total_time);
    
    //Print_matrix("Result Matrix", C, n, n);
    
    
    Lab1_saveoutput(C, &n, total_time);
    
    free(A);
    free(B);
    free(C);
    
    return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:       Pth_mat_mat
 * Purpose:        Multiply an mxn matrix by an nx1 column vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
 void *Pth_mat_mat(void* rank) {
     long my_rank = (long) rank;
     int i, j, k;
     int local_size = n/thread_count;
     
     int x = floor(my_rank/sqrt(thread_count));
     int y = (int) my_rank % (int) sqrt(thread_count);
     
     int my_first_row = (n/sqrt(thread_count))*x;
     int my_last_row = ((n/sqrt(thread_count))*(x+1))-1;
     
     int my_first_col = (n/sqrt(thread_count))*y;
     int my_last_col = ((n/sqrt(thread_count))*(y+1))-1;
     
     
     for(i = my_first_row; i <= my_last_row; i++)
     {
         for(j = my_first_col; j <= my_last_col; j++)
         {
             int sum = 0;
             for(k = 0; k < n; k++)
             {
                 sum += Get_matrix_value(A, i, k) * Get_matrix_value(B, k, j);
             }
             Set_matrix_value(&C, i, j, sum);
         }
     }
    return NULL;
 }  /* Pth_mat_mat */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
    fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
    exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Read_matrix
 * Purpose:     Read in the matrix
 * In args:     prompt, m, n
 * Out arg:     A
 */
void Read_matrix(char* prompt, int **A, int m, int n) {
    int             i, j;
    
    printf("%s\n", prompt);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &A[i][j]);
}  /* Read_matrix */


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix(char* title, int **A, int m, int n) {
    int   i, j;
    
    printf("%s\n", title);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }
}  /* Print_matrix */
