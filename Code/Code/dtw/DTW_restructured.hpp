/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       DTW_restructured.hpp
*   @desc       Header library for Dynamic Time Warping with restructured cost matrix memory
*   @author     Graham N. Power
*   @date       2023-12-14
*   @version    1.1.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Containes the following implementations of dynamic time warping
*
*   1. Memoization for O(2n) space complexity (DynamicTimeWarping_Memoization)
*   2. 
*
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-13 	Initial file implementation for restructured cost matrices
*   1.1.0   gnp     2023-12-14  Added matrix row & column index transformations for storing 
*                               the matrix row/column interleaved
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
#ifndef DTW_RESTRUCTURED_H_
#define DTW_RESTRUCTURED_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "float.h"
#include <time.h>
#include <math.h>
#include <iostream>

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
double DynamicTimeWarping_Memoization(ClassDataSingle y1, ClassDataSingle y2)
{
    unsigned int i, j;
    unsigned int mat_l = y1.size() + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(2 * mat_l * sizeof(double));
    for (j = 1; j < mat_l; j++)
    {
        cost_matrix[j] = DBL_MAX;
    }
    cost_matrix[mat_l] = DBL_MAX;
    cost_matrix[0] = 0;

    // Calculate the cost matrix
    double cost, prev_min;
    unsigned int p = 0;
    unsigned int c = 1;
    unsigned int tmp;
    for (i = 1; i < mat_l; i++)
    {
        for (j = 1; j < mat_l; j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            //cost = cost * cost;
            // Take the last min from the window
            prev_min = min(cost_matrix[(p)*mat_l + j], cost_matrix[(c)*mat_l + (j-1)]);
            prev_min = min(prev_min, cost_matrix[(p)*mat_l + (j-1)]);
            cost_matrix[(c)*mat_l + j] = cost + prev_min;            
        }
        // Swap c,p
        tmp = c;
        c = p;
        p = tmp;
    }
    cost = sqrt(cost_matrix[(2)*mat_l - 1]);

    free(cost_matrix);
    return cost;
};


// Store matrix row/column interleaved
unsigned int ij2k_rci(unsigned int n, unsigned int i, unsigned int j)
{
    if (i > j)
    {
        return ((n+1)*(n))/2 + (j*n + i - 2*j - 1) - (j*(j-1))/2;
    } else {
        return (i*n + j - i) - (i*(i-1))/2;
    }
};


/*******************************************************************************
*   Global Variables
*******************************************************************************/


/*******************************************************************************
*   Functions
*******************************************************************************/



/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_RESTRUCTURED_H_ */