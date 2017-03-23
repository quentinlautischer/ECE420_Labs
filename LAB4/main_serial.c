/* ECE 420 - LAB 4
 * 
 * PageRank
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "timer.h" // For speed testing
#include "Lab4_IO.c"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

void Solve();

/* Global variables */
int thread_count;

struct node *nodehead;
int nodecount;
int *in_links;
int *out_links;
double *R, *r_pre;

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

    printf("nodecount: %d\n", nodecount);
    printf("in_links: %d\n", *in_links);
    printf("out_links: %d\n", *out_links);

    GET_TIME(t_start);

    Solve();


    double t = report_timing(t_start, t_end);
    
    //Output
    Lab4_saveoutput(R, nodecount, t);
    printf("Output Generated\n");

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
    
    int i, j;
    double damp_const;
    int iterationcount = 0;
    int collected_nodecount;
    double *collected_r;
    double cst_addapted_threshold;
    double error;


    R = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        R[i] = 1.0 / nodecount;
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;
    // CORE CALCULATION


    do{
        ++iterationcount;
        vec_cp(R, r_pre, nodecount);
        for ( i = 0; i < nodecount; ++i){
            R[i] = 0;
            for ( j = 0; j < nodehead[i].num_in_links; ++j)
                R[i] += r_pre[nodehead[i].inlinks[j]] / out_links[nodehead[i].inlinks[j]];
            R[i] *= DAMPING_FACTOR;
            R[i] += damp_const;
        }
    }while(rel_error(R, r_pre, nodecount) >= EPSILON);


    printf("Finished\n");
            
    return;
 }
