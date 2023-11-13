
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "float.h"


#define min(a,b) \
    ({  __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

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
    double * cost_matrix = malloc(mat_l * mat_l * sizeof(double));
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

    return cost;
};

// Swapped for loops
double DynamicTimeWarping_1(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = malloc(mat_l * mat_l * sizeof(double));
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

    return cost;
};


// Reduce double bit widths
double DynamicTimeWarping_2(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    float * cost_matrix = malloc(mat_l * mat_l * sizeof(float));
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

    return cost;
};


// Initialize loop not completely needed
double DynamicTimeWarping_3(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    double * cost_matrix = malloc(mat_l * mat_l * sizeof(double));
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
    double * cost_matrix = malloc(mat_l * mat_l * sizeof(double));
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

    return cost;
};


// All 1,2,3 atthe same time
double DynamicTimeWarping_123(double * y1, double * y2, unsigned int ylength)
{
    unsigned int i, j;
    unsigned int mat_l = ylength + 1;
    // Initialize cost matrix
    float * cost_matrix = malloc(mat_l * mat_l * sizeof(float));
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

    return cost;
};

// DynamicTimeWarping  : Original
// DynamicTimeWarping_1: Swapped for loops
// DynamicTimeWarping_2: Reduce double bit width
// DynamicTimeWarping_3: Initialize loop not completely needed
// DynamicTimeWarping_5: Store matrix row/column interleaved
// TODO:
// DynamicTimeWarping_4: Multithread row and column
// DynamicTimeWarping_6: Multithread row and column by blocks


int main()
{
    double a[6] = {10, 11, 12, 13, 14, 15};
    double b[6] = {0, 0, 1, 2, 3, 4};

    double ans  = DynamicTimeWarping(a, b, 6);
    double ans1 = DynamicTimeWarping_1(a, b, 6);
    double ans2 = DynamicTimeWarping_2(a, b, 6);
    double ans3 = DynamicTimeWarping_3(a, b, 6);
    double ans5 = DynamicTimeWarping_5(a, b, 6);
    double ans123 = DynamicTimeWarping_123(a, b, 6);

    printf("%10.6f\n", ans);
    printf("%10.6f\n", ans1);
    printf("%10.6f\n", ans2);
    printf("%10.6f\n", ans3);
    printf("%10.6f\n", ans5);
    printf("%10.6f\n", ans123);
};