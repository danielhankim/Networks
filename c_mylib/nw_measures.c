#include <stdio.h>
#include <stdlib.h>

//#############################################################################
// Clustering coefficient
//#############################################################################

/* calculate the clustering coefficient of a single node */
double SingleCC(int **adj, int *deg, int N, int node) {
    int i, j, k, L=0;
    double Coef;

    for (i = 0; i < deg[node]; i++) {
        for (j = i + 1; j < deg[node]; j++) {
            for (k = 0; k < deg[adj[node][i]]; k++) {
                if (adj[adj[node][i]][k] == adj[node][j]) {
                    L += 1;
                }
            }
        }
    }
    Coef = (double) 2 * L / (deg[node] * (deg[node] - 1));
    return Coef;
}

/* calculate the clustering coefficient of the whole nodes */
int AllCCs(int **adj, int *deg, double *CC, int N) {
    int i, j, k, node, L;

    for (node = 0; node < N; node++) {
        if (deg[node] > 1) {
            for (L=0, i = 0; i < deg[node]; i++) {
                for (j = i + 1; j < deg[node]; j++) {
                    for (k = 0; k < deg[adj[node][i]]; k++) {
                        if (adj[adj[node][i]][k] == adj[node][j]) {
                            L += 1;
                        }
                    }
                }
            }
            CC[node] = (double) 2 * L / (deg[node] * (deg[node] - 1));
        } else {
            CC[node] = 0.;
        }
    }
    return 0; 
}
/* calculate the clustering coefficient of only designated nodes */
int PartialCCs(int **adj, int *deg, double *CC, int **nodes, int N_changes) {
    int n, i, j, k, node, L;

    for (n = 0; n < N_changes; n++) {
        node = nodes[0][n];
        if (deg[node] > 1) {
            for (L=0, i = 0; i < deg[node]; i++) {
                for (j = i + 1; j < deg[node]; j++) {
                    for (k = 0; k < deg[adj[node][i]]; k++) {
                        if (adj[adj[node][i]][k] == adj[node][j]) {
                            L += 1;
                        }
                    }
                }
            }
            CC[node] = (double) 2 * L / (deg[node] * (deg[node] - 1));
        } else {
            CC[node] = 0.;
        }
    }
    return 0; 
}

/* calculate the average clustering coefficient */
/* include zeros from degree 0, 1 nodes */
double AvgCC1(double *CC, int N) {
    int i;
    double AvgCC;

    for (AvgCC = 0., i = 0; i < N; i++) {
        AvgCC += CC[i];
    }
    AvgCC = AvgCC / (double) N;

    return AvgCC; 
}

/* calculate the average clustering coefficient */
/* exclude zeros from degree 0, 1 nodes */
double AvgCC2(int *deg, double *CC, int N) {
    int i, N_eff;
    double AvgCC;

    N_eff = N, AvgCC = 0.;  
    for (i = 0; i < N; i++) {
        AvgCC += CC[i];
        if (deg[i] < 2) {
            N_eff -= 1;
        }
    }
    AvgCC = AvgCC / (double) N_eff;
    return AvgCC; 
}

/* calculate the global clustering coefficient */
double GlobCC(int **adj, int *deg, int N) {
    int i, j, k, node, N_triangles=0, N_triplets=0;
    double GlobalCC;

    for (node = 0; node < N; node++) {
        for (i = 0; i < deg[node]; i++) {
            for (j = i + 1; j < deg[node]; j++) {
                for (k = 0; k < deg[adj[node][i]]; k++) {
                    if (adj[adj[node][i]][k] == adj[node][j]) {
                        N_triangles += 1;
                    }
                }
            }
        }
        N_triplets += deg[node] * (deg[node] - 1);
    }

    GlobalCC = (double) 2 * N_triangles / N_triplets;
    return GlobalCC;
}

//#############################################################################
// Centrality
//#############################################################################


//#############################################################################
// Length
//#############################################################################

//#############################################################################
// Assortativity mixing
//#############################################################################

//#############################################################################
// degree moment
//#############################################################################