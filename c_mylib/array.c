#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


//#############################################################################
// initialize 1d array to have "init_val"
//#############################################################################

void init_1d_int(int *ary, int max, int init_val) {
    int i;
    for (i = 0; i < max; i++) {
        ary[i] = init_val;
    }
}

void init_1d_double(double *ary, int max, double init_val) {
    int i;
    for (i = 0; i < max; i++) {
        ary[i] = init_val;
    }
}

//#############################################################################
// initialize 2d array to have "init_val"
//#############################################################################

void init_2d_int1(int **ary, int max1, int *max2, int init_val) {
    int i;
    for (i = 0; i < max1; i++) {
        ary[i] = (int *)calloc(max2[i], sizeof(int));
        if (init_val != 0) {
            init_1d_int(ary[i], max2[i], init_val);
        }
    }
}

void init_2d_int2(int **ary, int max1, int max2, int init_val) {
    int i;
    for (i = 0; i < max1; i++) {
        ary[i] = (int *)calloc(max2, sizeof(int));
        init_1d_int(ary[i], max2, init_val);
    }
}


//#############################################################################
// copy 1d array
//#############################################################################

void copy_1d_int1(int *to, int *from, int max) {
    int i;
    for (i = 0; i < max; i++) {
        to[i] = from[i];
    }
}

// copy 1d int arry excluding unnecessary values "x_val"
void copy_1d_int2(int *to, int *from, int max, int x_val) {
    int i, j;
    for (i = 0, j = 0; i < max; i++) {
        if (from[i] != x_val) {
            to[j] = from[i];
            j += 1;
        }
    }
}

void copy_1d_double1(double *to, double *from, int max) {
    int i;
    for (i = 0; i < max; i++) {
        to[i] = from[i];
    }
}

//#############################################################################
// sum values in 1d array
//#############################################################################

int sum_1d_int(int *ary, int max) {
    int i, sum;
    for (i = 0, sum = 0; i < max; i++) {sum += ary[i];}
    return sum;
}


double sum_1d_double(int *ary, int max) {
    int i;
    double sum;
    for (i = 0, sum = 0; i < max; i++) {sum += ary[i];}
    return sum;
}

//#############################################################################
// sum values in 2d array
//#############################################################################

int sum_2d_int1(int **ary, int max1, int *max2) {
    int i, j, sum;
    for (i = 0, sum = 0; i < max1; i++) {
        for (j = 0; j < max2[i]; j++) {
            sum += ary[i][j];
        }
    }
    return sum;
}

int sum_2d_int2(int **ary, int max1, int max2) {
    int i, j, sum;
    for (i = 0, sum = 0; i < max1; i++) {
        for (j = 0; j < max2; j++) {
            sum += ary[i][j];
        }
    }
    return sum;
}

int sum_2d_double1(double **ary, int max1, int *max2) {
    int i, j;
    double sum;
    for (i = 0, sum = 0; i < max1; i++) {
        for (j = 0; j < max2[i]; j++) {
            sum += ary[i][j];
        }
    }
    return sum;
}

int sum_2d_double2(double **ary, int max1, int max2) {
    int i, j;
    double sum;
    for (i = 0, sum = 0; i < max1; i++) {
        for (j = 0; j < max2; j++) {
            sum += ary[i][j];
        }
    }
    return sum;
}

//#############################################################################
// make average of a given array
//#############################################################################

double avg_1d_int(int *ary, int max) {
    int i, sum;
    double avg;
    for (i = 0, sum = 0; i < max; i++) {
        sum += ary[i];
    }
    avg = (double) sum / max;
    return avg;
}


double avg_1d_double(int *ary, int max) {
    int i;
    double sum, avg;
    for (i = 0, sum = 0; i < max; i++) {
        sum += ary[i];
    }
    avg = sum / (double) max;
    return avg;
}

//#############################################################################
// find index of an array with certain value "my_val"
//#############################################################################

int give_idx(int *ary, int max, int my_val) {
    int i;
    for (i = 0; i < max; i++) {
        if (ary[i] == my_val) {
            return i;
        }
    }
    return -1;
}
