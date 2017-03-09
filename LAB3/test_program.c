#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "timer.h" // For speed testing
#include <omp.h>

double report_timing(double start, double end) {
    GET_TIME(end);
    double total_time = (double) end - start;
    printf("It took %f seconds\n", total_time);
    return total_time;
}

int main(int argc, char* argv[]) {
    double     t_start = 0.0;
    double     t_end = 0.0;
    int A[2000000];

    GET_TIME(t_start);


    #pragma omp parallel for num_threads(400)
    for (int i = 0; i < 2000000; i++) {
    	A[i] = i;
    }


    double t = report_timing(t_start, t_end);
    

    
    return 0;
}  /* main */