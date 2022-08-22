#include <stdio.h>
#include <stdlib.h>

// file handling -> python
// 1. match file format (i:j -> i;j, i j -> i;j, etc.)
// 2. check starting index & adjust starting to 0
// 3. check missing nodes between node_0 and node_N-1
// 4. if nodes are missed, give good index

int get_nw_size(char *fn) {
    FILE *fp;
    int N, i, j;

    fp = fopen(fn, "r");
    if (fp == NULL) {
        printf("!!! FAIL TO OPEN YOUR FILE !!! \n");
        printf("!!! CHECK YOUR FILE AGAIN  !!! \n");
        return -1;
    } else {
        N=-1;
        do {
            fscanf(fp, "%d;%d\n", &i, &j);
            if(N <= i) N=i;
            if(N <= j) N=j;

        } while(!feof(fp));
        fclose(fp);
    }

    N += 1;

    return N;
}

int get_adj_list(char *fn, int **adj, int *deg, int N) {
    FILE *fp;
    int i, j;

    fp = fopen(fn, "r");
    if (fp == NULL) {
        printf("!!! FAIL TO OPEN YOUR FILE !!! \n");
        printf("!!! CHECK YOUR FILE AGAIN  !!! \n");
        return -1;
    } else {
        do{
            fscanf(fp, "%d;%d\n", &i, &j);
            deg[i]++;
            adj[i] = (int *)realloc(adj[i], sizeof(int) * deg[i]);
            adj[i][deg[i] - 1] = j;
        }while(!feof(fp));
        fclose(fp);
    }

    return 0;
}


// int save_adj_data(char *fn, int **adj, int *deg, int N) {
//     FILE *fp;
//     int i, j;

//     fp = fopen(fn, "w");

//     if (fp == NULL) {
//         printf("!!! FAIL TO MAKE YOUR FILE !!! \n");
//         printf("!!! CHECK YOUR FILE AGAIN  !!! \n");
//         return -1;
//     } else {
//         for (i = 0; i < N; i++) {
//             for (j = 0; j < deg[i]; j++) {
//                 fprintf(fp, "%d;%d\n", i, adj[i][j]);
//             }
//         }
//     }
//     fclose(fp);

//     return 0;
// }

// save degree information of a given network
int save_pk_data(char *fn, int *deg, int N) {
    FILE *fp;
    int i;

    fp = fopen(fn, "w");

    if (fp == NULL) {
        printf("!!! FAIL TO MAKE YOUR FILE !!! \n");
        printf("!!! CHECK YOUR FILE AGAIN  !!! \n");
        return -1;
    } else {
        for (i = 0; i < N; i++) {
            fprintf(fp, "%d\n", deg[i]);
        }
    }
    fclose(fp);
    return 0;
}

// save degree information of a given network
int save_deg_data(char *fn, int *deg, int N) {
    FILE *fp;
    int i;

    fp = fopen(fn, "w");

    if (fp == NULL) {
        printf("!!! FAIL TO MAKE YOUR FILE !!! \n");
        printf("!!! CHECK YOUR FILE AGAIN  !!! \n");
        return -1;
    } else {
        for (i = 0; i < N; i++) {
            fprintf(fp, "%d\n", deg[i]);
        }
    }
    fclose(fp);
    return 0;
}

// save degree information of an ensemble of networks
int save_multiple_deg_data(char *fn, int *deg, int N) {
    FILE *fp;
    int i;

    fp = fopen(fn, "a"); // !!! this is the only difference !!!

    if (fp == NULL) {
        printf("!!! FAIL TO MAKE YOUR FILE !!! \n");
        printf("!!! CHECK YOUR FILE AGAIN  !!! \n");
        return -1;
    } else {
        for (i = 0; i < N; i++) {
            fprintf(fp, "%d\n", deg[i]);
        }
    }
    fclose(fp);
    return 0;
}



