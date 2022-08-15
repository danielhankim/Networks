#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

//#include "network_models.h"

//#############################################################################
// Basic Lattice model
//#############################################################################


void make_1D_Ring_w_PBC();

void make_1d_Ring_wo_PBC();

void make_2D_Square_w_PBC();

void make_2D_Square_wo_PBC();

void make_BetheLattice();


//#############################################################################
// Basic Network Models
//#############################################################################

void make_CGraph(int **adj, int *deg, int nw_size) {
    int i, j, idx;
    init_1d_int(deg, nw_size, nw_size - 1);
    init_2d_int2(adj, nw_size, nw_size - 1, 0);
    for (i = 0; i < nw_size; i++) { 
        for (idx = 0, j = 0; j < nw_size; j++) {
            if (i != j) {
                adj[i][idx] = j;
                idx += 1;
            }
        }
    }
}

void make_ErdosRenyi(int **adj, int *deg, int nw_size, double mean_k) {

    int n_link, n_tot_link, node1, node2, ticket, i;
    double rnd;
    init_1d_int(deg, nw_size, 0);
    init_2d_int1(adj, nw_size, deg, -1);
    size_t intsz;
    intsz = sizeof(int);

    n_tot_link = (int) (nw_size * mean_k / 2);
    while (n_link < n_tot_link) {
        rnd = genrand64_real2();
        node1 = (int) (nw_size * rnd);
        rnd = genrand64_real2();
        node2 = (int) (nw_size * rnd);
        if (node1 != node2) {
            for (ticket = 0, i = 0; i < deg[node1]; i++) {
                if (adj[node1][i] == node2) {
                    ticket = 1;
                    break;
                }
            }
            if (ticket == 0) {
                deg[node1] += 1;
                deg[node2] += 1;
                adj[node1] = (int *)realloc(adj[node1], intsz * deg[node1]); 
                adj[node2] = (int *)realloc(adj[node2], intsz * deg[node2]);
                adj[node1][deg[node1] - 1] = node2;
                adj[node2][deg[node2] - 1] = node1;
                n_link += 1;
            }
        } 
    }
}

void make_SNU_ScaleFree(int **adj, int *deg, int nw_size, double k_mean, double gamma) {

    int *id, *poor, *rich, i, j, ticket,
    n_poor, n_rich, n_tot_link, n_link, node1, node2, tmp;
    double *prob, alpha, sum_prob, prob_avg, rnd;
    size_t intsz;
    intsz = sizeof(int);

    alpha = 1.0 / (gamma - 1.0);
    init_1d_int(deg, nw_size, 0);
    init_2d_int1(adj, nw_size, deg, -1);

    id = (int *)calloc(nw_size, intsz);
    prob = (double *)calloc(nw_size, intsz);

    rich = (int *)calloc(0, intsz);
    poor = (int *)calloc(0, intsz);
    
    for (i = 0, sum_prob = 0.; i < nw_size; i++) {
        prob[i] = pow((double)(i+1), -alpha);
        id[i] = i;
        sum_prob += prob[i];
    }
    
    for (i = 0; i < nw_size; i++) {
        prob[i] = prob[i] / sum_prob * (double) nw_size;
    }

    prob_avg = 1.;

    for (i = 0, n_poor = 0, n_rich = 0; i < nw_size; i++) {
        if (prob[i] > prob_avg) {
            n_rich += 1;
            rich = (int *)realloc(rich, n_rich * intsz);
            rich[n_rich - 1] = i;
        } else {
            n_poor += 1;
            poor = (int *)realloc(poor, n_poor * intsz);
            poor[n_poor - 1] = i;
        }
    }

    while (1) {
        prob[rich[n_rich - 1]] -= (prob_avg - prob[poor[n_poor - 1]]);
        id[poor[n_poor - 1]] = rich[n_rich - 1];
        n_poor -= 1;
        poor = (int *)realloc(poor, n_poor * intsz);

        if (prob[rich[n_rich - 1]] < prob_avg) {
            n_poor += 1;
            poor = (int *)realloc(poor, n_poor * intsz);
            poor[n_poor - 1] = rich[n_rich - 1];
            n_rich -= 1;
            rich = (int *)realloc(rich, n_rich * intsz);
        } else if (prob[rich[n_rich - 1]] == prob_avg) {
            n_rich -= 1;
            rich = (int *)realloc(rich, n_rich * intsz);
        }

        if (n_rich == 0 || n_poor == 0) {break;}

        n_tot_link = (int) ((double) nw_size * k_mean / 2.);
        n_link = 0;

        while (1) {
            rnd = genrand64_real2();
            tmp = (int) (nw_size * rnd);
            rnd = genrand64_real2();
            if (prob[tmp] > rnd) {node1 = tmp;} 
            else {node1 = id[tmp];}
            rnd = genrand64_real2();
            tmp = (int) (nw_size * rnd);
            rnd = genrand64_real2();
            if (prob[tmp] > rnd) {node2 = tmp;} 
            else {node2 = id[tmp];}

            if (node1 != node2) {
                for (ticket = 0, i = 0; i < deg[node1]; i++) {
                    if (node2 == adj[node1][i]) {
                        ticket = 1;
                        break;
                    }
                }
                if (ticket == 0) {
                    deg[node1] += 1;
                    deg[node2] += 1;
                    adj[node1] = (int *)realloc(adj[node1], intsz * deg[node1]); 
                    adj[node2] = (int *)realloc(adj[node2], intsz * deg[node2]);
                    adj[node1][deg[node1] - 1] = node2;
                    adj[node2][deg[node2] - 1] = node1;
                    n_link += 1; 
                }

            }
        }
        if (n_link == n_tot_link) {break;}
    }
    free(poor); free(rich);
    free(prob); free(id);
}

void make_BarabasiAlbert();

void make_WattsStrogatz();

void make_Configuration(int **adj, int *deg, double *cdf, int nw_size, int k_max, int k_min) {
    int *stubs, *stub_list, *cpy_stub_list, n, n_stubs, node1, node2, idx1, idx2, i, j, k;
    double rnd;

    stubs = (int *)calloc(nw_size, sizeof(int));

    // Assign stubs to each node
    for (i = 0; i < nw_size; i++) {
        rnd = genrand64_real2();
        for (j = 0; j < k_max - k_min + 1; j++) {
            if (rnd <= cdf[j]) {
                stubs[i] = k_min + j;
                break;
            }
        }
    }
    // Add extra stub when number of total stubs is odd
    n_stubs = sum_1d_int(stubs, nw_size);
    if (n_stubs % 2 == 1) {
        rnd = genrand64_real2();
        i = (int) (nw_size * rnd);
        stubs[i] += 1;
        n_stubs += 1;
    }

    copy_1d_int1(deg, stubs, nw_size);
    init_2d_int1(adj, nw_size, deg, -1);
    stub_list = (int *) calloc(n_stubs, sizeof(int));
    cpy_stub_list = (int *)calloc(n_stubs, sizeof(int));

    // make list of stubs to pick
    for (i = 0; i < nw_size; i++) {
        for (j = 0; j < stubs[i]; j++){
            stub_list[k] = i;
            k +=1;
        }
    }

    while (1) {
        // while (1) {
        rnd = genrand64_real2();
        node1 = stub_list[(int)(n_stubs * rnd)];
            // if (node1 != -1) {break;}
        // }
        stub_list[(int)(n_stubs * rnd)] = -1; // remove used stub
        stubs[node1] -= 1;
        // while (1) {
        rnd = genrand64_real2();
        node2 = stub_list[(int)(n_stubs * rnd)];
            // if (node2 != -1) {break;}
        // }
        stub_list[(int)(n_stubs * rnd)] = -1; // remove used stub
        stubs[node2] -= 1;

        idx1 = give_idx(adj[node1], deg[node1], -1);
        adj[node1][idx1] = node2;
        idx2 = give_idx(adj[node2], deg[node2], -1);
        adj[node2][idx2] = node1;

        // remove -1 in stub_list for efficiency
        n_stubs -= 2;
        if (n_stubs == 0) {break;}
        cpy_stub_list = (int *)realloc(cpy_stub_list, sizeof(int) * n_stubs);
        copy_1d_int2(cpy_stub_list, stub_list, n_stubs + 2, -1);
        stub_list = (int *)realloc(stub_list, sizeof(int) * n_stubs);
        copy_1d_int1(stub_list, cpy_stub_list, n_stubs);
    }
    free(stubs); free(stub_list), free(cpy_stub_list);
}