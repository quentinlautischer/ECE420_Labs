/* ECE 420 - LAB 4
 * 
 * PageRank
 */

#define LAB4_EXTEND

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "timer.h" // For speed testing
#include "Lab4IO.h"

void Solve();

/* Global variables */
int thread_count;

struct node *nodehead;
int nodecount;
int *in_links;
int *out_links;
double *R;


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

    thread_count = 8;

    //Load
    get_node_stat(&nodecount, &in_links, &out_links);
    node_init(&nodehead, in_links, out_links, 0, nodecount);

    Solve();

    double t = report_timing(t_start, t_end);
    
    //Output
    Lab4_saveoutput(R, nodecount, t);

    //Free Memory
    node_destroy(nodehead, nodecount);
    free(in_links); 
    free(out_links);
    return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:       Solve
 * Purpose:        
 * In arg:         
 * Global in vars: 
 * Global out var: 
 */
 void Solve() {
    node n;





    

    
            
    return;
 }
