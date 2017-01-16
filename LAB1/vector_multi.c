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

double* A;
double* B;
double* C;

/* Serial functions */
void Usage(char* prog_name);
void Read_matrix(char* prompt, double A[], int m, int n);
void Print_matrix(char* title, double A[], int m, int n);

/* Parallel function */
void *Pth_mat_vect(void* rank);

// Added functions
bool Is_square(int num){
   return sqrt(num) == sqrt(num);  
}

bool Is_divisor(int num, int numerator){
   return num <= sqrt(numerator) && numerator % num == 0;
}

/*------------------------------------------------------------------
 * Function:  Find_largest_square_divisor
 * Purpose:   returns the largest possible divisor that is square for the int requested_thread_count 
 * In arg :   prog_name
 */
int Find_largest_square_divisor (int requested_thread_count, int matrix_dim){
   int x = requested_thread_count;
   while(x>0){
      if (Is_divisor(x, matrix_dim*matrix_dim) && Is_square(x))
         return x;
      --x;
   }
   return 0;
}

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   time_t     t_start;
   time_t     t_end;
   long       thread;
   pthread_t* thread_handles;

   if (argc != 2) Usage(argv[0]);
  
   printf("Enter n \n");
   scanf("%d", &n);

   A = malloc(n*n*sizeof(double));
   B = malloc(n*n*sizeof(double));
   C = calloc(n*n, sizeof(double));
   
   Lab1_loadinput(&A, &B, &n);

   thread_count = Find_largest_square_divisor(atoi(argv[1]), n);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   printf("Thread Count: %i\n", thread_count);

   GET_TIME(t_start);
   
   // for (thread = 0; thread < thread_count; thread++)
   //    pthread_create(&thread_handles[thread], NULL,
   //       Pth_mat_mat, (void*) thread);

   // for (thread = 0; thread < thread_count; thread++)
   //    pthread_join(thread_handles[thread], NULL);
   
   GET_TIME(t_end);
   double total_time = (double) t_end - t_start;

   printf("It took %f seconds\n", total_time);
   Print_matrix("The product is", C, n, n);
  
   Lab1_saveoutput(&C, &n, total_time);

   free(A);
   free(B);
   free(C);

   return 0;
}  /* main */

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
void Read_matrix(char* prompt, double A[], int m, int n) {
   int             i, j;

   printf("%s\n", prompt);
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}  /* Read_matrix */


/*------------------------------------------------------------------
 * Function:        Read_vector
 * Purpose:         Read in the vector x
 * In arg:          prompt, n
 * Out arg:         x
 */
void Read_vector(char* prompt, double x[], int n) {
   int   i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}  /* Read_vector */


/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        Multiply an mxn matrix by an nx1 column vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
// void *Pth_mat_mat(void* rank) {
//    long my_rank = (long) rank;
//    int i, j;
//    int local_m = m/thread_count; 
//    int my_first_row = my_rank*local_m;
//    int my_last_row = (my_rank+1)*local_m - 1;

//    for (i = my_first_row; i <= my_last_row; i++) {
//       y[i] = 0.0;
//       for (j = 0; j < n; j++)
//           y[i] += A[i*n+j]*x[j];
//    }

//    return NULL;
// }  /* Pth_mat_vect */


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix( char* title, double A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
}  /* Print_matrix */


/*------------------------------------------------------------------
 * Function:    Print_vector
 * Purpose:     Print a vector
 * In args:     title, y, m
 */
void Print_vector(char* title, double y[], double m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}  /* Print_vector */
