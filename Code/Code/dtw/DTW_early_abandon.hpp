/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       DTW_early_abandon.hpp
*   @desc       Header library for Dynamic Time Warping with early abandoning
*   @author     Graham N. Power
*   @date       2023-12-15
*   @version    1.1.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Containes the following implementations of dynamic time warping
*
*   1. Warp constraints for single DTW comparison (DynamicTimeWarping_WarpConstrained)
*   2. Early abandoned to quit for too large a distance (DynamicTimeWarping_EarlyAbandoned)
*   3. Pruned DTW from https://sites.labic.icmc.usp.br/dfs/pdf/SDM_PrunedDTW.pdf (DynamicTimeWarping_Pruned)
*
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-13 	Initial file implementation for just warp constrained DTW
*   1.1.0   gnp     2023-12-15  Added early abandoned and lower bounds DTW
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
#ifndef DTW_EARLY_ABANDON_H_
#define DTW_EARLY_ABANDON_H_


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
typedef struct
{
    int i;
    int j;
} ExpandedWindowEntry_t;


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
double DynamicTimeWarping_WarpConstrained(ClassDataSingle y1, ClassDataSingle y2, int w)
{
    int i, j;
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
        //std::cout << "i(" << i << ") j(" << j << ") i-w(" << i-w << ") i+w(" << i+w << ") max_i-w(" << max(1, i-w) << ") min_i+w(" << min(mat_l, i+w) << ")" << std::endl;     
        for (j = max(1, i-w); j < min((int)mat_l, i+w+1); j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }

    //PrintMatrix(cost_matrix, mat_l);

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


double DynamicTimeWarping_WarpConstrained(ClassDataSingle y1, ClassDataSingle y2)
{
    return DynamicTimeWarping_WarpConstrained(y1, y2, max(y1.size(), y2.size()));
};


double DynamicTimeWarping_EarlyAbandoned(ClassDataSingle y1, ClassDataSingle y2, double c)
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
        if (cost > c)
        {
            free(cost_matrix);
            return DBL_MAX;
        }
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


double DynamicTimeWarping_EarlyAbandoned(ClassDataSingle y1, ClassDataSingle y2)
{
    return DynamicTimeWarping_EarlyAbandoned(y1, y2, DBL_MAX);
};


double DynamicTimeWarping_Pruned(ClassDataSingle y1, ClassDataSingle y2, double ws, double ub)
{
    // std::cout << "Allocating cost matrix" << std::endl;
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

    // std::cout << "Calculating cost matrix" << std::endl;
    // std::cout << "Size Me:" << std::endl;
    // std::cout << y1.size() << ", " << y2.size() << ", " << mat_l * mat_l << std::endl;
    // Calculate the cost matrix
    double cost, prev_min;
    int sc = 1;
    int ec = 1;
    unsigned int beg, end, ec_next;
    bool smallerFound;
    for (i = 1; i < mat_l; i++)
    {
        beg = max((int)sc, (int)(i-ws));
        end = min((double)(i+ws), (double)mat_l);
        // std::cout << "i_ws: " << (double)(i+ws) << ", y1s: " << (double)mat_l << ", end: " << end << std::endl;
        smallerFound = false;
        ec_next = i;

        for (j = beg; j < end; j++)
        {
            // std::cout << i << ", " << j << std::endl;
            // std::cout << "A" << std::endl;
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            // std::cout << "B" << std::endl;
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            // std::cout << "C" << std::endl;
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            // std::cout << "D" << std::endl;
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
            // Employ pruning strategy
            // std::cout << "E" << std::endl;
            if (cost_matrix[(i)*mat_l + j] > ub)
            {
                if (smallerFound == false)
                {
                    sc = j + 1;
                }
                if (j >= ec)
                {
                    break;
                }
            }
            else
            {
                smallerFound = true;
                ec_next = j + 1;
            }
            // std::cout << std::endl;
            // End of pruning strategy
        }

        ec = ec_next;
    }

    // std::cout << "Indexing cost matrix" << std::endl;
    cost = cost_matrix[(mat_l*mat_l)-1];

    // std::cout << "Free cost matrix" << std::endl;
    free(cost_matrix);
    return cost;
};


double DynamicTimeWarping_Pruned(ClassDataSingle y1, ClassDataSingle y2)
{
    double sqED = 0;
    for (int i = 0; i < y1.size(); i++)
    {
        sqED += (y1[i] - y2[i])*(y1[i] - y2[i]);
    }
    return DynamicTimeWarping_Pruned(y1, y2, DBL_MAX, sqED);
};


/*
ClassDataSingle ClassDataSingle_ReduceByHalf(ClassDataSingle y)
{
    ClassDataSingle ynew;
    for (int i = 0; i < y.size() / 2; i += 2)
    {
        ynew.push_back((y[i] + y[i+1])/2);
    }

    return ynew;
};


std::vector<ExpandedWindowEntry_t> ExpandWindow(std::vector<ExpandedWindowEntry_t> & path, ClassDataSingle y1, ClassDataSingle y2, int searchRadius)
{
    for (int k = 0; k < path.size(); k++)
    {
        std::vector<ExpandedWindowEntry_t> z((searchRadius+2)*(searchRadius+2));
        unsigned int idx = 0;
        for (int a = -searchRadius; a < searchRadius + 1; a++)
        {
            for (int b = -searchRadius; b < searchRadius + 1; b++)
            {
                z[idx].i = path[k].i + a;
                z[idx].j = path[k].j + b;
                idx++;
            }
        }
        for (int i = 0; i < z.size(); i++)
        {
            path.push_back(z[i]);
        }
    }

    std::vector<ExpandedWindowEntry_t> window_i;
    for (int k = 0; k < path.size(); k++)
    {
        window_i.push_back({.i = (path[k].i * 2), .j = (path[k].j * 2)});
        window_i.push_back({.i = (path[k].i * 2), .j = ((path[k].j * 2) + 1)});
        window_i.push_back({.i = ((path[k].i * 2) + 1), .j = (path[k].j * 2)});
        window_i.push_back({.i = ((path[k].i * 2) + 1), .j = ((path[k].j * 2) + 1)});
    }

    std::vector<ExpandedWindowEntry_t> window;
    int startJ = 0;
    for (int i = 0; i < y1.size(); i++)
    {
        int newStartJ = -1;
        for (int j = startJ; j < y2.size(); j++)
        {
            ExpandedWindowEntry_t ij = {.i = i, .j = j};
            if (std::find(window_i.begin(), window_i.end(), ij) != window_i.end())
            {
                window.push_back(ij);
                if (newStartJ < 0)
                {
                    newStartJ = j;
                }
            }
            else if (newStartJ >= 0)
            {
                break;
            }
        }
        startJ = newStartJ;
    }

    return window;
};


double FastDTW_Internal(ClassDataSingle y1, ClassDataSingle y2, std::vector<ExpandedWindowEntry_t> window, double distance, std::vector<ExpandedWindowEntry_t> & lowResPath)
{
    if (window.empty())
    {
        for (int i = 0; i < y1.size(); i++)
        {
            for (int j = 0; j < y2.size(); j++)
            {
                window.push_back({.i = i, .j = j});
            }
        }
    }
    
    for(int k = 0; k < window.size(); k++)
    {
        window[k].i++;
        window[k].j++;
    }
};


double DynamicTimeWarping_Fast(ClassDataSingle y1, ClassDataSingle y2, int searchRadius, std::vector<ExpandedWindowEntry_t> & lowResPath)
{
    int minTSize = searchRadius + 2;

    // Base Case: for a very small time series run the full DTW algorithm.
    if (y1.size() <= minTSize || y2.size() <= minTSize)
    {
        return DynamicTimeWarping_Basic(y1, y2);
    }

    // Recursive Case: Project the warp path from
    // a coarser resolution onto the current
    // current resolution. Run DTW only along
    // the projected path (and also ‘radius’ cells
    // from the projected path).
    ClassDataSingle shrunkY1 = ClassDataSingle_ReduceByHalf(y1);
    ClassDataSingle shrunkY2 = ClassDataSingle_ReduceByHalf(y2);

    double distance = DynamicTimeWarping_Fast(shrunkY1, shrunkY2, searchRadius, lowResPath);

    std::vector<ExpandedWindowEntry_t> window = ExpandWindow(lowResPath, y1, y2, searchRadius);

    return FastDTW_Internal(y1, y2, window, distance, lowResPath);
};

double DynamicTimeWarping_Fast(ClassDataSingle y1, ClassDataSingle y2, int searchRadius)
{
    
};
*/

/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_EARLY_ABANDON_H_ */