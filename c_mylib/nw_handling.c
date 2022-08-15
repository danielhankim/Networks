#include <stdio.h>
#include <stdlib.h>


//#############################################################################
// Burning Algorithm
//#############################################################################
void burning(int **adj, int *deg, int *cluster_id, int nw_size) {
    int *node_list, *stack, i, j, k, id, rp, end, seed, new, remainder;
    double rnd;
    size_t intsz;
    intsz = sizeof(int);

    node_list = (int *)calloc(nw_size, intsz);
    stack = (int *)calloc(1, intsz);
    init_1d_int(cluster_id, nw_size, -1);
    for (i = 0; i < nw_size; i++) {node_list[i] = i;}
    remainder = nw_size;
    
    id = 0;
    while (1) {
        rp = 0;
        rnd = genrand64_real2();
        seed = node_list[(int) (remainder * rnd)];
        if (cluster_id[seed] == -1) {
            cluster_id[seed] = id;
            end = deg[seed];
            stack = (int *)realloc(stack, intsz * end);
            copy_1d_int1(stack, adj[seed], end);
            
            // assign cluster id
            for (i = 0; i < deg[seed]; i++) {
                cluster_id[adj[seed][i]] = id;
            }

            while (rp != end) {
                for (i = 0; i < deg[stack[rp]]; i++) {
                    new = adj[stack[rp]][i];
                    if (cluster_id[new] == -1) {
                        end += 1;
                        stack = (int *)realloc(stack, intsz * end);
                        stack[end - 1] = new;
                        cluster_id[new] = id;
                    }
                }
                rp += 1;
            }
        }
        id += 1;

        // count remainder
        for (remainder = 0, i = 0; i < nw_size; i++) {
            if (cluster_id[i] == -1) {
                remainder += 1;
            }
        }
        if (remainder == 0) {break;}
        node_list = (int *)realloc(node_list, intsz * remainder);
        
        // make list of remainder nodes
        for (i = 0, j = 0; i < nw_size; i++) {
            if (cluster_id[i] == -1) {
                node_list[j] = i;
                j += 1;
            }
        }
    }
    
    free(node_list), free(stack);
}

int extract_lc(int **adj, int *deg, int **new_adj, int *new_deg, int nw_size) {
    int *table, *cluster_size, *cluster_id;
    int n_cluster, lc_size, lc_id, i, j, k, idx1, idx2, count, max;
    size_t intsz;
    intsz = sizeof(int);
    cluster_id = calloc(nw_size, intsz);
    burning(adj, deg, cluster_id, nw_size);
 
    // find id and size of largest cluster
    for (n_cluster = 0, i = 0; i < nw_size; i++) {
        if (cluster_id[i] >=  n_cluster) {
            n_cluster = cluster_id[i];
        }
    }
    n_cluster += 1;

    cluster_size = (int *)calloc(n_cluster, intsz);
    for (i = 0; i < nw_size; i++) {cluster_size[cluster_id[i]] += 1;}
    
    for (lc_size = -1, i = 0; i < n_cluster; i++) {
        if (cluster_size[i] >= lc_size) {
            lc_size = cluster_size[i];
        }
    }
    lc_id = give_idx(cluster_size, n_cluster, lc_size);
     
    printf("nw size: %d ---> %d\n", nw_size, lc_size);

    // adjust index label due to burning process
    table = (int *)calloc(nw_size, intsz); 
    for (i = 0, count = 0; i < nw_size; i++) {
        if (cluster_id[i] == lc_id) {
            table[i] = i - count;
        } else {
            table[i] = -1;
            count += 1;
        }
    }

    for (i = 0, j = 0; i < nw_size; i++) {
        if (cluster_id[i] == lc_id) {
            new_deg[j] = deg[i];
            j += 1;            
        }
    }

    init_2d_int1(new_adj, nw_size, new_deg, 0);

    idx1 = 0, idx2 = 0;
    for (i = 0; i < nw_size; i++) {
        if (cluster_id[i] == lc_id) {
            for (j = 0; j < deg[i]; j++) {
                if (cluster_id[adj[i][j]] == lc_id) {
                    new_adj[idx1][idx2] = table[adj[i][j]];
                    idx2 += 1;
                }
            }
            idx1 += 1, idx2 = 0;
        }
    }
    free(table); free(cluster_size);


    return lc_size;
}


//#############################################################################
// Rewiring method
//#############################################################################
/* Rewiring Links preserving degree distribution */
void link_rewiring1(int **adj, int *deg, int N) {
    int *candidate1, *candidate2, *final1, *final2, node1, node2, 
    friend1, friend2, i, j, n_candidate1, n_candidate2;
    double rnd;

    /* choose two nodes who will swap their friends */
    rnd = genrand64_real2();
    node1 = (int) (rnd * N);
    while (1) {
        rnd = genrand64_real2();
        node2 = (int) (rnd * N);
        if (node1 != node2) {
            break;
        }
    }

    /* make candidate list of node1 and node2 to swap */
    n_candidate1 = deg[node1], n_candidate2 = deg[node2];
    candidate1 = (int *)calloc(n_candidate1, sizeof(int));
    candidate2 = (int *)calloc(n_candidate2, sizeof(int));
  
    for (i = 0; i < deg[node1]; i++) {
        candidate1[i] = adj[node1][i];
        for (j = 0; j < deg[node2]; j++) {
            if ((candidate1[i] == adj[node2][j]) || (candidate1[i] == node2)) {
                n_candidate1 -= 1;
                candidate1[i] = -1;
            }
        }
    }
    if (n_candidate1 == 0) {
        free(candidate1), free(candidate2);
        return -1; 
    } else {
        final1 = (int *)calloc(n_candidate1, sizeof(int));
        for (i = 0, j = 0; i < deg[node1]; i++) {
            if (candidate1[i] != -1) {
                final1[j] = candidate1[i];
                j += 1;
            }
        }
    }

    for (i = 0; i < deg[node2]; i++) {
        candidate2[i] = adj[node2][i];
        for (j = 0; j < deg[node1]; j++) {
            if ((candidate2[i] == adj[node1][j]) || (candidate2[i] == node1)) {
                n_candidate2 -= 1;
                candidate2[i] = -1;
            }
        }
    }
    if (n_candidate2 == 0) {
        free(candidate1), free(candidate2);
        return -1; 
    } else {
        final2 = (int *)calloc(n_candidate2, sizeof(int));
        for (i = 0, j = 0; i < deg[node2]; i++) {
            if (candidate2[i] != -1) {
                final2[j] = candidate2[i];
                j += 1;
            }
        }
    }
    
    /* choose the final friends to swap */
    rnd = genrand64_real2();
    friend1 = final1[(int) (rnd * n_candidate1)];
    rnd = genrand64_real2();
    friend2 = final2[(int) (rnd * n_candidate2)];

    /* Information Update */
    for (i = 0; i < deg[node1]; i++) {
        if (adj[node1][i] == friend1) {
            adj[node1][i] = friend2;
            break;
        }
    }
    for (i = 0; i < deg[node2]; i++) {
        if (adj[node2][i] == friend2) {
            adj[node2][i] = friend1;
            break;
        }
    }

    for (i = 0; i < deg[friend1]; i++) {
        if (adj[friend1][i] == node1) {
            adj[friend1][i] = node2;
            break;
        }
    }

    for (i = 0; i < deg[friend2]; i++) {
        if (adj[friend2][i] == node2) {
            adj[friend2][i] = node1;
            break;
        }
    } 
    
    free(candidate1), free(candidate2);
    free(final1), free(final2);
}

/* Rewiring Links without preserving degree distribution */
/* Sometimes, this makes zero-degree nodes */
int link_rewiring2(int **adj, int *deg, int N) {
    int *copy_adj, node1, node2, friend, 
    i, j, k, idx, check, n_trial;

    node1 = (int) (genrand64_real2() * N);
    friend = adj[node1][(int) (genrand64_real2() * deg[node1])];

    
    n_trial = 0;
    while (1) {
        printf("n_trial = %d\n", n_trial);
        check = 0;
        node2 = (int) (genrand64_real2() * N);
        if (friend != node2) {
            for (i = 0; i < deg[node2]; i++) {
                if (friend == adj[node2][i]) {
                    check = +1;
                }
            }
            if (check == 0) {
                deg[node2] += 1;
                adj[node2] = (int *)realloc(adj[node2], deg[node2]);
                adj[node2][deg[node2]-1] = friend;

                for (i = 0; i < deg[friend]; i++) {
                    if (adj[friend][i] == node1) {
                        adj[friend][i] = node2;
                    }
                }

                copy_adj = (int *)calloc(deg[node1], sizeof(int));
                for (i = 0, j = 0; i < deg[node1]; i++) {
                    if (adj[node1][i] != friend) {
                        copy_adj[j] = adj[node1][i];
                        j += 1;
                    }
                }
                deg[node1] -= 1;
                adj[node1] = (int *)realloc(copy_adj, deg[node1]);
                break;
            }
        }
        n_trial += 1;
    }
    // printf("original:\n");
    // printf("%d-%d\n", node1, friend);
    // printf("after:\n");
    // printf("%d-%d\n", node2, friend);

    // free(copy_adj);
    return 0;
}


/* Rewiring Links without preserving degree distribution */
/* No zero-degree nodes are created here */
int link_rewiring3(int **adj, int *deg, int N) {
    int *copy_adj, node1, node2, friend, 
    i, j, k, idx, check, n_trial;

    while (1) {
        node1 = (int) (genrand64_real2() * N);
        if (deg[node1] > 1) {
            friend = adj[node1][(int) (genrand64_real2() * deg[node1])];
            break;
        }
    }
    
    n_trial = 0;
    while (1) {
        printf("n_trial = %d\n", n_trial);
        check = 0;
        node2 = (int) (genrand64_real2() * N);
        if (friend != node2) {
            for (i = 0; i < deg[node2]; i++) {
                if (friend == adj[node2][i]) {
                    check = +1;
                }
            }
            if (check == 0) {
                deg[node2] += 1;
                adj[node2] = (int *)realloc(adj[node2], deg[node2]);
                adj[node2][deg[node2]-1] = friend;

                for (i = 0; i < deg[friend]; i++) {
                    if (adj[friend][i] == node1) {
                        adj[friend][i] = node2;
                    }
                }

                copy_adj = (int *)calloc(deg[node1], sizeof(int));
                for (i = 0, j = 0; i < deg[node1]; i++) {
                    if (adj[node1][i] != friend) {
                        copy_adj[j] = adj[node1][i];
                        j += 1;
                    }
                }
                deg[node1] -= 1;
                adj[node1] = (int *)realloc(copy_adj, deg[node1]);
                break;
            }
        }
        n_trial += 1;
    }
    // printf("original:\n");
    // printf("%d-%d\n", node1, friend);
    // printf("after:\n");
    // printf("%d-%d\n", node2, friend);

    // free(copy_adj);
    return 0;
}
/* LinkRewiring1 + gives swap information */
int link_rewiring1_plus(int **adj, int **change, int *deg, int N) {
    int *candidate1, *candidate2, *final1, *final2, node1, node2, 
    friend1, friend2, i, j, n_candidate1, n_candidate2;
    double rnd;

    /* choose two nodes who will swap their friends */
    node1 = (int) (genrand64_real2() * N);
    while (1) {
        node2 = (int) (genrand64_real2() * N);
        if (node1 != node2) {
            break;
        }
    }

    /* make candidate list of node1 and node2 to swap */
    n_candidate1 = deg[node1], n_candidate2 = deg[node2];
    candidate1 = (int *)calloc(n_candidate1, sizeof(int));
    candidate2 = (int *)calloc(n_candidate2, sizeof(int));
  
    for (i = 0; i < deg[node1]; i++) {
        candidate1[i] = adj[node1][i];
        for (j = 0; j < deg[node2]; j++) {
            if ((candidate1[i] == adj[node2][j]) || (candidate1[i] == node2)) {
                n_candidate1 -= 1;
                candidate1[i] = -1;
            }
        }
    }
    if (n_candidate1 == 0) {
        free(candidate1), free(candidate2);
        return -1; 
    } else {
        final1 = (int *)calloc(n_candidate1, sizeof(int));
        for (i = 0, j = 0; i < deg[node1]; i++) {
            if (candidate1[i] != -1) {
                final1[j] = candidate1[i];
                j += 1;
            }
        }
    }

    for (i = 0; i < deg[node2]; i++) {
        candidate2[i] = adj[node2][i];
        for (j = 0; j < deg[node1]; j++) {
            if ((candidate2[i] == adj[node1][j]) || (candidate2[i] == node1)) {
                n_candidate2 -= 1;
                candidate2[i] = -1;
            }
        }
    }
    if (n_candidate2 == 0) {
        free(candidate1), free(candidate2);
        return -1; 
    } else {
        final2 = (int *)calloc(n_candidate2, sizeof(int));
        for (i = 0, j = 0; i < deg[node2]; i++) {
            if (candidate2[i] != -1) {
                final2[j] = candidate2[i];
                j += 1;
            }
        }
    }
    
    /* choose the final friends to swap */
    friend1 = final1[(int) (genrand64_real2() * n_candidate1)];
    friend2 = final2[(int) (genrand64_real2() * n_candidate2)];

    /* Information Update */
    for (i = 0; i < deg[node1]; i++) {
        if (adj[node1][i] == friend1) {
            adj[node1][i] = friend2;
            break;
        }
    }
    for (i = 0; i < deg[node2]; i++) {
        if (adj[node2][i] == friend2) {
            adj[node2][i] = friend1;
            break;
        }
    }

    for (i = 0; i < deg[friend1]; i++) {
        if (adj[friend1][i] == node1) {
            adj[friend1][i] = node2;
            break;
        }
    }

    for (i = 0; i < deg[friend2]; i++) {
        if (adj[friend2][i] == node2) {
            adj[friend2][i] = node1;
            break;
        }
    }
    change[0] = (int *)calloc(4, sizeof(int));
    change[0][0] = node1, change[0][1] = node2;
    change[0][2] = friend1, change[0][3] = friend2;
    
    free(candidate1), free(candidate2);
    free(final1), free(final2);
    return 0;
}

//#############################################################################
// Correction or Simplification
//#############################################################################
int count_self_loop(int **adj, int *deg, int nw_size) {
    int count, i, j;
    for (i = 0, count = 0; i < nw_size; i++) {
        for (j = 0; j < deg[i]; j++) {
            if (adj[i][j] == i) {
                count += 1;
            }
        }
    }
    count = (int) (count / 2);
    printf("number of self-loop(s): %d\n", count);
    return count;
}

int count_multi_loop(int **adj, int *deg, int nw_size) {
    int count, i, j, k;
    for (i = 0, count = 0; i < nw_size; i++) {
        for (j = 0; j < deg[i]; j++) {
            for (k = j + 1; k < deg[i]; k++) {
                if (adj[i][j] == adj[i][k]) {
                    count += 1;
                }
            }
        }
    }
    count = (int) (count / 2);
    printf("number of multi-links(s): %d\n", count);
    return count;
}

int check_symmetry(int **adj, int *deg, int nw_size) {
    int i, j, k, node1, node2, ticket;

    for (i = 0; i < nw_size; i++) {
        node1 = i;
        for (j = 0; j < deg[i]; j++) {
            node2 = adj[i][j];
            for (ticket = 0, k = 0; k < deg[node2]; k++) {
                if (node1 == adj[node2][k]) {ticket += 1;}
            }
            if (ticket == 0) {
                printf("network is assymetric!\n");
                return -1;
            }
            if (ticket > 1) {
                printf("multi-link detected!\n");
                return -1;
            }
        }
    }
    printf("network is perfectly symmetric!\n");
    return 0;
}

void remove_self_loop(int **adj, int *deg, int nw_size) {
    int **new_adj, *new_deg, *table, i, j, count;
    new_adj = (int **)calloc(nw_size, sizeof(int *));
    new_deg = (int *)calloc(nw_size, sizeof(int)); 
    table = (int *)calloc(nw_size, sizeof(int));
    
    for (i = 0; i < nw_size; i++) {
        for (j = 0; j < deg[i]; j++) {
            if (adj[i][j] == i) {adj[i][j] = -1;}
            else {
                new_deg[i] += 1;
                new_adj[i] = (int *)realloc(new_adj[i], sizeof(int) * new_deg[i]);
                new_adj[i][new_deg[i] - 1] = adj[i][j];
            }
        }
    }

    // give good index
    for (count = 0, i = 0; i < nw_size; i++) {
        if (new_deg[i] == 0) {
            table[i] = -1;
            count += 1;
        } else {
            table[i] = i - count;
        }
    }

    for (i = 0; i < nw_size; i++) {
        for (j = 0; j < new_deg[i]; j++) {
            new_adj[i][j] = table[new_adj[i][j]];
        }
    } 

    init_1d_int(deg, nw_size, 0);

    copy_1d_int2(deg, new_deg, nw_size, 0);
    for (count = 0, i = 0; i < nw_size; i++) {
        adj[i] = (int *)realloc(adj[i], sizeof(int) * deg[i]);
        if (new_deg[i] == 0) {
            count += 1;
        } else {
            for (j = 0; j < new_deg[i]; j++) {
                adj[i - count][j] = new_adj[i][j];
            } 
        } 
    }

    for (i = 0; i < nw_size; i++) {free(new_adj[i]);} 
    free(new_deg); 
    free(new_adj);
    free(table); 
}

void remove_multi_loop(int **adj, int *deg, int nw_size) {
    int  **new_adj, *new_deg, *table, i, j, k, count;
    new_adj = (int **)calloc(nw_size, sizeof(int *));
    new_deg = (int *)calloc(nw_size, sizeof(int)); 
    table = (int *)calloc(nw_size, sizeof(int));

    for (i = 0; i < nw_size; i++) {
        for (j = 0; j < deg[i]; j++) {
            if (adj[i][j] != -1) {
                for (k = j+1; k < deg[i]; k++) {
                    if (adj[i][j] == adj[i][k]) {
                        adj[i][j] = -1;
                    }
                }
            }
        }
    }

    for (i = 0; i < nw_size; i++) {
        for (j = 0; j < deg[i]; j++) {
            if (adj[i][j] != -1) {
                new_deg[i] += 1;
                new_adj[i] = (int *)realloc(new_adj[i], sizeof(int) * new_deg[i]);
                new_adj[i][new_deg[i] - 1] = adj[i][j];
            }
        }
    } 

    // give good index
    for (count = 0, i = 0; i < nw_size; i++) {
        if (new_deg[i] == 0) {
            table[i] = -1;
            count += 1;
        } else {
            table[i] = i - count;
        }
    }

    for (i = 0; i < nw_size; i++) {
        for (j = 0; j < new_deg[i]; j++) {
            new_adj[i][j] = table[new_adj[i][j]];
        }
    } 

    init_1d_int(deg, nw_size, 0);
    copy_1d_int2(deg, new_deg, nw_size, 0);
    for (count = 0, i = 0; i < nw_size; i++) {
        adj[i] = (int *)realloc(adj[i], sizeof(int) * deg[i]);
        if (new_deg[i] == 0) {
            count += 1;
        } else {
            for (j = 0; j < new_deg[i]; j++) {
                adj[i - count][j] = new_adj[i][j];
            } 
        } 
    }

    for (i = 0; i < nw_size; i++) {free(new_adj[i]);} 
    free(new_deg); 
    free(new_adj);
    free(table);  
}

// unweighted graph with self-loops or muli-links -> simple graph
void make_simple_graph1(int **adj, int *deg, int nw_size) {

}



//#############################################################################
// Renormalization Transformation
//#############################################################################


//#############################################################################
// K-core decomposition 
//#############################################################################