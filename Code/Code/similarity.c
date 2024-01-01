
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "float.h"
#include <time.h>
#include <math.h>


#define min(a,b) \
    ({  __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

#define max(a,b) \
    ({  __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })

uint8_t Pipe_Abs(int * y1, int * y2, unsigned int ylength, double pipe_range)
{
    int max, min;
    unsigned int i;
    for (i = 0; i < ylength; i++)
    {
        max = y1[i] + pipe_range;
        min = y1[i] - pipe_range;
        if (y2[i] > max || y2[i] < min)
        {
            return 0x00;
        }
    }
    return 0xFF;
};


uint8_t Pipe_Rel(double * y1, double * y2, unsigned int ylength, double pipe_range)
{
    double max, min;
    unsigned int i;
    for (i = 0; i < ylength; i++)
    {
        max = y1[i] + (y1[i]*pipe_range);
        min = y1[i] - (y1[i]*pipe_range);
        if (y2[i] > max || y2[i] < min)
        {
            return 0x00;
        }
    }
    return 0xFF;
};


double MeanSquareError(double * y1, double * y2, unsigned int ylength)
{
    double mse, diff;
    unsigned int i;
    for (i = 0; i < ylength; i++)
    {
        diff = y1[i] - y2[i];
        mse += diff*diff;
    }
    return mse / ylength;
};


double MeanAbsoluteError(double * y1, double * y2, unsigned int ylength)
{
    double abe;
    unsigned int i;
    for (i = 0; i < ylength; i++)
    {
        abe += abs(y1[i] - y2[i]);
    }
    return abe / ylength;
};


double CrossCorrelation(double * x, double * y1, double * y2, unsigned int ylength)
{
    return 0;
};

void PrintMatrix(double * mat, unsigned int ylength)
{
    unsigned int i, j;
    for (i = 0; i < ylength; i++)
    {
        for (j = 0; j < ylength; j++)
        {
            printf("%10.6f  ", mat[i*ylength + j]);
        }
        printf("\n");
    }
    printf("\n\n\n\n");
};

double DynamicTimeWarping(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(mat_l * mat_l * sizeof(double));
    for (i = 0; i < mat_l; i++)
    {
        for (j = 0; j < mat_l; j++)
        {
            cost_matrix[(i)*mat_l + j] = DBL_MAX;
        }
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    double cost, prev_min;
    for (i = 1; i < mat_l; i++)
    {
        for (j = 1; j < mat_l; j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    double mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = DBL_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = DBL_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = DBL_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};

// Swapped for loops
double DynamicTimeWarping_1(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(mat_l * mat_l * sizeof(double));
    for (i = 0; i < mat_l; i++)
    {
        for (j = 0; j < mat_l; j++)
        {
            cost_matrix[(i)*mat_l + j] = DBL_MAX;
        }
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    double cost, prev_min;
    for (j = 1; j < mat_l; j++)
    {
        for (i = 1; i < mat_l; i++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    double mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = DBL_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = DBL_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = DBL_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};


// Reduce double bit widths
double DynamicTimeWarping_2(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    float * cost_matrix = (float *) malloc(mat_l * mat_l * sizeof(float));
    for (i = 0; i < mat_l; i++)
    {
        for (j = 0; j < mat_l; j++)
        {
            cost_matrix[(i)*mat_l + j] = FLT_MAX;
        }
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    float cost, prev_min;
    for (i = 1; i < mat_l; i++)
    {
        for (j = 1; j < mat_l; j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    float mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = FLT_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = FLT_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = FLT_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};


// Initialize loop not completely needed
double DynamicTimeWarping_3(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(mat_l * mat_l * sizeof(double));
    for (i = 1; i < mat_l; i++)
    {
        cost_matrix[(0)*mat_l + i] = DBL_MAX;
        cost_matrix[(i)*mat_l + 0] = DBL_MAX;
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    double cost, prev_min;
    for (i = 1; i < mat_l; i++)
    {
        for (j = 1; j < mat_l; j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    double mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = DBL_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = DBL_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = DBL_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};






// Store matrix row/column interleaved
unsigned int ij2k(unsigned int n, unsigned int i, unsigned int j)
{
    if (i > j)
    {
        return ((n+1)*(n))/2 + (j*n + i - 2*j - 1) - (j*(j-1))/2;
    } else {
        return (i*n + j - i) - (i*(i-1))/2;
    }
};

double DynamicTimeWarping_5(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(mat_l * mat_l * sizeof(double));
    for (i = 0; i < mat_l; i++)
    {
        for (j = 0; j < mat_l; j++)
        {
            cost_matrix[ij2k(ylength, i, j)] = DBL_MAX;
        }
    }
    cost_matrix[ij2k(ylength, 0, 0)] = 0;

    // Calculate the cost matrix
    double cost, prev_min;
    for (i = 1; i < mat_l; i++)
    {
        for (j = 1; j < mat_l; j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[ij2k(ylength, i-1, j)], cost_matrix[ij2k(ylength, i, j-1)]);
            prev_min = min(prev_min, cost_matrix[ij2k(ylength, i-1, j-1)]);
            cost_matrix[ij2k(ylength, i, j)] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    double mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[ij2k(ylength, l_i, l_j)];
        if (l_i > 0) {
            mi = cost_matrix[ij2k(ylength, l_i-1, l_j)];
        } else {
            mi = DBL_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[ij2k(ylength, l_i, l_j-1)];
        } else {
            mj = DBL_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[ij2k(ylength, l_i-1, l_j-1)];
        } else {
            mij = DBL_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[ij2k(ylength, l_i, l_j)];

    free(cost_matrix);
    return cost;
};


// Limit W
double DynamicTimeWarping_6(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(mat_l * mat_l * sizeof(double));
    for (i = 0; i < mat_l; i++)
    {
        for (j = 0; j < mat_l; j++)
        {
            cost_matrix[(i)*mat_l + j] = DBL_MAX;
        }
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    double cost, prev_min;
    unsigned int w = 20;
    for (i = 1; i < mat_l; i++)
    {
        for (j = max(2, i-w); j < min(ylength+1, i+w); j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    double mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = DBL_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = DBL_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = DBL_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};


// All 1,2,3 atthe same time
double DynamicTimeWarping_123(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    float * cost_matrix = (float *) malloc(mat_l * mat_l * sizeof(float));
    for (i = 1; i < mat_l; i++)
    {
        cost_matrix[(0)*mat_l + i] = FLT_MAX;
        cost_matrix[(i)*mat_l + 0] = FLT_MAX;
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    float cost, prev_min;
    for (j = 1; j < mat_l; j++)
    {
        for (i = 1; i < mat_l; i++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    float mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = FLT_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = FLT_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = FLT_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};


// All 2,3,6 at the same time
double DynamicTimeWarping_236(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    float * cost_matrix = (float *) malloc(mat_l * mat_l * sizeof(float));
    for (i = 1; i < mat_l; i++)
    {
        cost_matrix[(0)*mat_l + i] = FLT_MAX;
        cost_matrix[(i)*mat_l + 0] = FLT_MAX;
    }
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    float cost, prev_min;
    unsigned int w = 20;
    for (i = 1; i < mat_l; i++)
    {
        for (j = max(2, i-w); j < min(ylength+1, i+w); j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    // Walk the cost matrix
    cost = 0;
    int l_i = ylength;
    int l_j = ylength;
    float mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        cost += cost_matrix[(l_i)*mat_l + l_j];
        if (l_i > 0) {
            mi = cost_matrix[(l_i-1)*mat_l + l_j];
        } else {
            mi = FLT_MAX;
        }
        if (l_j > 0) {
            mj = cost_matrix[(l_i)*mat_l + l_j-1];
        } else {
            mj = FLT_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
        } else {
            mij = FLT_MAX;
        }
        
        if (mi <= mj && mi <= mij) {
            l_i--;
        } else if (mj <= mi && mj <= mij) {
            l_j--;
        } else {
            l_i--;
            l_j--;
        }
    }
    cost += cost_matrix[(l_i)*mat_l + l_j];

    free(cost_matrix);
    return cost;
};

// ✓ DynamicTimeWarping  : Original
// ✓ DynamicTimeWarping_1: Swapped for loops
// ✓ DynamicTimeWarping_2: Reduce double bit width
// ✓ DynamicTimeWarping_3: Initialize loop not completely needed
// ✓ DynamicTimeWarping_6: Limit warping
// TODO:
// (unknown benefit https://www.cs.unm.edu/~mueen/DTW.pdf)
// DynamicTimeWarping_#: Memoization ??
// DynamicTimeWarping_4: Multithread row and column
// DynamicTimeWarping_5: Store matrix row/column interleaved
// DynamicTimeWarping_5: Multithread diagonal
// DynamicTimeWarping_5: Store matrix by diagonal rows
// (tempo c++)
// DynamicTimeWarping_7: Early Abandoned
// DynamicTimeWarping_8: Pruned
// DynamicTimeWarping_9: LB_Keogh
// DynamicTimeWarping_10: LB_Enhanced
// DynamicTimeWarping_11: LB_Webb
// (UltraFastMP java)
// DynamicTimeWarping_12: Fast Warping ?? 
// (lbimproved c++)
// DynamicTimeWarping_13: fast nearest negihbor retrieval
// (fastdtw java)
// DynamicTimeWarping_14: Fast DTW ??

// DynamicTimeWarping_#: Multithread row and column by blocks

#define NUM_IMPLEMENTATIONS     8
#define MONTECARLO_RUNS         10
#define NUM_VECSIZES            14 // 12 => 4096

typedef double (*DTW_i)(double * y1, double * y2, unsigned int ylength);

typedef struct {
    unsigned int vecSizes[NUM_VECSIZES];
    DTW_i funcs[NUM_IMPLEMENTATIONS];
    char * funcNames[NUM_IMPLEMENTATIONS];
    double results[NUM_IMPLEMENTATIONS][NUM_VECSIZES];
} ExecutionTime_t;

void TimeDTWImplementations(ExecutionTime_t * results);
void PrintDTWResults(ExecutionTime_t * results);

int main()
{
    // double a[6] = {10, 11, 12, 13, 14, 15};
    // double b[6] = {0, 0, 1, 2, 3, 4};

    // double ans  = DynamicTimeWarping(a, b, 6);
    // double ans1 = DynamicTimeWarping_1(a, b, 6);
    // double ans2 = DynamicTimeWarping_2(a, b, 6);
    // double ans3 = DynamicTimeWarping_3(a, b, 6);
    // double ans5 = DynamicTimeWarping_5(a, b, 6);
    // double ans123 = DynamicTimeWarping_123(a, b, 6);

    // printf("%10.6f\n", ans);
    // printf("%10.6f\n", ans1);
    // printf("%10.6f\n", ans2);
    // printf("%10.6f\n", ans3);
    // printf("%10.6f\n", ans5);
    // printf("%10.6f\n", ans123);

    // CALCULATE EXECUTION TIMES
    printf("Create results struct...\n");
    ExecutionTime_t times;

    // Set vecsizes based on 2**i
    printf("Load VecSizes...\n\t");
    unsigned int i;
    for (i = 0; i < NUM_VECSIZES; i++)
    {
        times.vecSizes[i] = pow(2, i+1);
        printf("%d, ", times.vecSizes[i]);
    }
    printf("\n");

    printf("Load funcs to time...\n");
    // Set the functions to time
    times.funcs[0] = DynamicTimeWarping;
    times.funcNames[0] = "Original";
    times.funcs[1] = DynamicTimeWarping_1;
    times.funcNames[1] = "Swapped Loops";
    times.funcs[2] = DynamicTimeWarping_2;
    times.funcNames[2] = "Small Bitwidth";
    times.funcs[3] = DynamicTimeWarping_3;
    times.funcNames[3] = "Partial Init";
    times.funcs[4] = DynamicTimeWarping_5;
    times.funcNames[4] = "Row/Col Interleave";
    times.funcs[5] = DynamicTimeWarping_123;
    times.funcNames[5] = "1&2&3";
    times.funcs[6] = DynamicTimeWarping_6;
    times.funcNames[6] = "Limit Warping";
    times.funcs[7] = DynamicTimeWarping_236;
    times.funcNames[7] = "2&3&6";

    // Call the timing code
    TimeDTWImplementations(&times);

    // Print the results
    PrintDTWResults(&times);
};


double drnad()
{
    return ( (double)rand() * (100) ) / (double)RAND_MAX;
};

double gmean(clock_t * befores, clock_t * afters, unsigned int length)
{
    double gmean_res;
    double product = 1;
    unsigned int i;
    for (i = 0; i < length; i++)
    {
        double diff = (double)(afters[i] - befores[i]);
        // printf("\t\t\tBefore: %d  Aftter: %d   Diff: %f\n", befores[i], afters[i], diff);
        if (diff > 0)
        {
            product = product * diff;
        }        
    }
    gmean_res = round(pow(product, 1./length));
    // printf("\t\t\tGMean: %f\n", gmean_res);
    return gmean_res;
};

void TimeDTWImplementations(ExecutionTime_t * results)
{
    printf("Beggining DTW Timing...\n");
    unsigned int i, j, k;

    // // Create random input vectors
    double * veca = malloc(results->vecSizes[NUM_VECSIZES-1] * sizeof(double));
    double * vecb = malloc(results->vecSizes[NUM_VECSIZES-1] * sizeof(double));
    for (i = 0; i < results->vecSizes[NUM_VECSIZES-1]; i++)
    {
        veca[i] = drnad();
        vecb[i] = drnad();
    }

    // Get timing results
    clock_t before[MONTECARLO_RUNS];
    clock_t after[MONTECARLO_RUNS];
    for (i = 0; i < NUM_VECSIZES; i++)
    {
        printf("\tVecsize: %d of %d\n", i+1, NUM_VECSIZES);
        for (j = 0; j < NUM_IMPLEMENTATIONS; j++)
        {
            printf("\t\tImpl: %d of %d\n", j+1, NUM_IMPLEMENTATIONS);
            for (k = 0; k <  MONTECARLO_RUNS; k++)
            {
                printf("\t\t\tMonteCarlo: %d of %d\n", k+1, MONTECARLO_RUNS);
                before[k] = clock();
                results->funcs[j](veca, vecb, results->vecSizes[i]);
                after[k] = clock();
                // printf("\t\t\t\tBefore: %d  After: %d\n", before[k], after[k]);
            }
            results->results[j][i] = gmean(before, after, MONTECARLO_RUNS);
            // printf("\t\t\tGMean2: %f\n", results->results[j][i]);
        }
    }

    free(veca);
    free(vecb);
    printf("\n\n\n");
};

void PrintDTWResults(ExecutionTime_t * results)
{
    printf("Timing Results:\n\n");
    unsigned int i, j;

    printf("Clocks Per Second: %d\n\n", CLOCKS_PER_SEC);

    // Print Header
    printf("Vector Size, ");
    for (i = 0; i < NUM_IMPLEMENTATIONS; i++)
    {
        printf("%s, ", results->funcNames[i]);
    }
    printf("\n");

    // Print Data
    for (i = 0; i < NUM_VECSIZES; i++)
    {
        printf("%d, ", results->vecSizes[i]);
        for (j = 0; j < NUM_IMPLEMENTATIONS; j++)
        {
            printf("%f, ", results->results[j][i]);
        }
        printf("\n");
    }
    printf("\n\n\n");
};
