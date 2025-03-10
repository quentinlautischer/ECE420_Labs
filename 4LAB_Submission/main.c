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
#include "mpi.h"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

#define MAX_STRING 250

void Solve();

/* Global variables */
int thread_count;

struct node *nodehead;
int nodecount;
int npes;
int *in_links;
int *out_links;
double *R, *r_pre;

double report_timing(double start, double end, int rank) {
    GET_TIME(end);
    double total_time = (double) end - start;
    printf("It took %f seconds\n", total_time);
    return total_time;
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {

    int myrank;
    double     t_start = 0.0;
    double     t_end = 0.0;

    MPI_INIT(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Load
    get_node_stat(&nodecount, &in_links, &out_links);
    node_init(&nodehead, in_links, out_links, 0, nodecount);

    if (myrank == 0) {
        printf("nodecount: %d\n", nodecount);
        printf("in_links: %d\n", *in_links);
        printf("out_links: %d\n", *out_links);
    }

    GET_TIME(t_start);

    Solve(myrank, npes);

    if (myrank == 0) {
        double t = report_timing(t_start, t_end, myrank);
        Lab4_saveoutput(R, nodecount, t);
        printf("Output Generated in proc %d\n", myrank);
    }
    //Free Memory
    node_destroy(nodehead, nodecount);
    free(in_links); 
    free(out_links);

    MPI_Finalize();

    return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:       Solve
 * Purpose:        
 * In arg:         
 * Global in vars: 
 * Global out var: 
 */
 void Solve(int rank, int npes) {
    int i, j;
    double damp_const;

    double *localR;
    int nodes_per_proc = nodecount/npes;

    R = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));

    localR = malloc(nodes_per_proc * sizeof(double));
    
    MPI_Scatter(R, nodes_per_proc, MPI_DOUBLE, localR, nodes_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (i = 0; i < nodes_per_proc; i++) {
        localR[i] = 1.0 / nodecount;
    }
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount; 

    MPI_Gather(localR, nodes_per_proc, MPI_DOUBLE, R, nodes_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    printf("Process %d gathered\n", rank);


    // // CORE CALCULATION
    // if (rank == 0) {
    // do{
    //     vec_cp(R, r_pre, nodecount);
    //     for ( i = 0; i < nodecount; i++){
    //         R[i] = 0;
    //         for ( j = 0; j < nodehead[i].num_in_links; j++)
    //             R[i] += r_pre[nodehead[i].inlinks[j]] / out_links[nodehead[i].inlinks[j]];
    //         R[i] *= DAMPING_FACTOR;
    //         R[i] += damp_const;
    //     }
    // } while(rel_error(R, r_pre, nodecount) >= EPSILON);
    // }

    int ii;
    int interation_count = 0;
    // CORE CALCULATION MPI
    vec_cp(R, r_pre, nodecount);
    int workMore = 1; 
    while(workMore) {
        interation_count++;
        // printf("Process %d starting work round: %d\n", rank, interation_count);
        MPI_Scatter(R, nodes_per_proc, MPI_DOUBLE, localR, nodes_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(r_pre, nodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for ( i = (nodes_per_proc*rank); i < (nodes_per_proc*(rank+1)); i++){
            ii = i-(nodes_per_proc*rank);
            localR[ii] = 0;
            for ( j = 0; j < nodehead[i].num_in_links; j++)
                localR[ii] += r_pre[nodehead[i].inlinks[j]] / out_links[nodehead[i].inlinks[j]];
            localR[ii] *= DAMPING_FACTOR;
            localR[ii] += damp_const;
        }
        MPI_Gather(localR, nodes_per_proc, MPI_DOUBLE, R, nodes_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // printf("Process %d finished work round: %d\n", rank, interation_count);

        //Check if more work
        if (rank == 0) {
            if (rel_error(R, r_pre, nodecount) >= EPSILON) {
                vec_cp(R, r_pre, nodecount);
                workMore = 1;
                MPI_Bcast(&workMore, 1, MPI_INT, 0, MPI_COMM_WORLD);
            } else {
                workMore = 0;
                MPI_Bcast(&workMore, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }
        } else {
            MPI_Bcast(&workMore, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

    printf("Finished working\n");
    return;
 }
