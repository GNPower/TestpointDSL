/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       DTW_basic.hpp
*   @desc       Header library for Dynamic Time Warping with early abandoning
*   @author     Graham N. Power
*   @date       2023-12-9
*   @version    1.0.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Containes the following implementations of dynamic time warping
*
*   1. Basic implementation (DynamicTimeWarping_Basic)
*   2. Swapped for loops (DynamicTimeWarping_SwappedLoops)
*   3. Reduced bit-widths from double to float (DynamicTimeWarping_ReducedBitWidths)
*   4. Skip initializing the unnecessary parts of the cost matrix (DynamicTimeWarping_PartialCostInit)
*
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-9 	Initial file implementation for basic DTW in cpp
*
*
*
*   0.0.0   -   Initial release
*   0.0.1   -   Patched some bug
*   0.1.0   -   Incremental change
*   1.0.0   -   First major release
*
*	</pre>
*
*******************************************************************************/


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#ifndef DTW_BASIC_H_
#define DTW_BASIC_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "float.h"
#include <time.h>
#include <math.h>

#include "DTW_datasets.hpp"


/*******************************************************************************
*   Preprocessor Macros
*******************************************************************************/


/*******************************************************************************
*   Datatype Definitions
*******************************************************************************/


/*******************************************************************************
*   Constant Definitions
*******************************************************************************/


/*******************************************************************************
*   Function Protoypes
*******************************************************************************/


/*******************************************************************************
*   Global Variables
*******************************************************************************/


/*******************************************************************************
*   Functions
*******************************************************************************/
double DynamicTimeWarping_Basic(ClassDataSingle y1, ClassDataSingle y2)
{
    unsigned int i, j;
    unsigned int mat_l = y1.size() + 1;
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
    int l_i = y1.size();
    int l_j = y1.size();
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


double DynamicTimeWarping_SwappedLoops(ClassDataSingle y1, ClassDataSingle y2)
{
    unsigned int i, j;
    unsigned int mat_l = y1.size() + 1;
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
    int l_i = y1.size();
    int l_j = y1.size();
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


double DynamicTimeWarping_ReducedBitWidths(ClassDataSingle y1, ClassDataSingle y2)
{
    unsigned int i, j;
    unsigned int mat_l = y1.size() + 1;
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
    int l_i = y1.size();
    int l_j = y1.size();
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


double DynamicTimeWarping_PartialCostInit(ClassDataSingle y1, ClassDataSingle y2)
{
    unsigned int i, j;
    unsigned int mat_l = y1.size() + 1;
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
    int l_i = y1.size();
    int l_j = y1.size();
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


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_BASIC_H_ */
