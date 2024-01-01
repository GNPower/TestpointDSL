/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       DTW_threaded.hpp
*   @desc       Header library for Dynamic Time Warping with multi-threading
*   @author     Graham N. Power
*   @date       2023-12-14
*   @version    1.0.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Containes the following implementations of dynamic time warping
*
*   1. Multi-threaded row & columns (DynamicTimeWarping_ThreadRowCol)
*   2. Multi-threaded row & columns where the cost matrix is stored row/column interleaved (DynamicTimeWarping_ThreadRowColInterleaved)
*
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-14 	Initial file implementation for multi-threaded DTW
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
#ifndef DTW_MULTITHREADED_H_
#define DTW_MULTITHREADED_H_


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
#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
#include <thread>
#endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)

#include "DTW_datasets.hpp"
#include "DTW_restructured.hpp"


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
void __DTW_ThreadSingleRow(ClassDataSingle y1, ClassDataSingle y2, double * cost_matrix, unsigned int mat_l, int i)
{
    // Calculate the cost matrix
    unsigned int j;
    double cost, prev_min;
    for(j = i; j < mat_l; j++)
    {
        cost = abs(y1[i-1] - y2[j-1]);
        // Take the last min from the window
        prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
        prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
        cost_matrix[(i)*mat_l + j] = cost + prev_min;
    }
};


void __DTW_ThreadSingleCol(ClassDataSingle y1, ClassDataSingle y2, double * cost_matrix, unsigned int mat_l, int j)
{
    // Calculate the cost matrix
    unsigned int i;
    double cost, prev_min;
    for(i = j+1; i < mat_l; i++)
    {
        cost = abs(y1[i-1] - y2[j-1]);
        // Take the last min from the window
        prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
        prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
        cost_matrix[(i)*mat_l + j] = cost + prev_min;
    }
};


void __DTW_ThreadSingleRow_RCI(ClassDataSingle y1, ClassDataSingle y2, double * cost_matrix, unsigned int mat_l, int i)
{
    // Calculate the cost matrix
    unsigned int j;
    double cost, prev_min;
    for(j = i; j < mat_l; j++)
    {
        cost = abs(y1[i-1] - y2[j-1]);
        // Take the last min from the window        
        //prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
        prev_min = min(cost_matrix[ij2k_rci(mat_l, i-1, j)], cost_matrix[ij2k_rci(mat_l, i, j-1)]);
        //prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
        prev_min = min(prev_min, cost_matrix[ij2k_rci(mat_l, i-1, j-1)]);
        //cost_matrix[(i)*mat_l + j] = cost + prev_min;
        cost_matrix[ij2k_rci(mat_l, i, j)] = cost + prev_min;
    }
};


void __DTW_ThreadSingleCol_RCI(ClassDataSingle y1, ClassDataSingle y2, double * cost_matrix, unsigned int mat_l, int j)
{
    // Calculate the cost matrix
    unsigned int i;
    double cost, prev_min;
    for(i = j+1; i < mat_l; i++)
    {
        cost = abs(y1[i-1] - y2[j-1]);
        // Take the last min from the window
        //prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
        prev_min = min(cost_matrix[ij2k_rci(mat_l, i-1, j)], cost_matrix[ij2k_rci(mat_l, i, j-1)]);
        //prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
        prev_min = min(prev_min, cost_matrix[ij2k_rci(mat_l, i-1, j-1)]);
        //cost_matrix[(i)*mat_l + j] = cost + prev_min;
        cost_matrix[ij2k_rci(mat_l, i, j)] = cost + prev_min;
    }
};


double DynamicTimeWarping_ThreadRowCol(ClassDataSingle y1, ClassDataSingle y2)
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
    unsigned int k;
    for (k = 1; k < mat_l; k++)
    {
        // TODO: Thread this!
        #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        std::thread t_row(__DTW_ThreadSingleRow, y1, y2, cost_matrix, mat_l, k);
        std::thread t_col(__DTW_ThreadSingleCol, y1, y2, cost_matrix, mat_l, k);
        t_row.join();
        t_col.join();
        #else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        __DTW_ThreadSingleRow(y1, y2, cost_matrix, mat_l, k);
        __DTW_ThreadSingleCol(y1, y2, cost_matrix, mat_l, k);
        #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    };

    // Walk the cost matrix
    double cost = 0;
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


double DynamicTimeWarping_ThreadRowColInterleaved(ClassDataSingle y1, ClassDataSingle y2)
{
    unsigned int i, j;
    unsigned int mat_l = y1.size() + 1;
    // Initialize cost matrix
    double * cost_matrix = (double *) malloc(mat_l * mat_l * sizeof(double));
    for (i = 0; i < mat_l; i++)
    {
        for (j = 0; j < mat_l; j++)
        {
            //cost_matrix[(i)*mat_l + j] = DBL_MAX;
            cost_matrix[ij2k_rci(mat_l, i, j)] = DBL_MAX;            
        }
    }
    //cost_matrix[0] = 0;
    cost_matrix[ij2k_rci(mat_l, 0, 0)] = 0;

    // Calculate the cost matrix
    unsigned int k;
    for (k = 1; k < mat_l; k++)
    {
        // TODO: Thread this!
        #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        std::thread t_row(__DTW_ThreadSingleRow_RCI, y1, y2, cost_matrix, mat_l, k);
        std::thread t_col(__DTW_ThreadSingleCol_RCI, y1, y2, cost_matrix, mat_l, k);
        t_row.join();
        t_col.join();
        #else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        __DTW_ThreadSingleRow_RCI(y1, y2, cost_matrix, mat_l, k);
        __DTW_ThreadSingleCol_RCI(y1, y2, cost_matrix, mat_l, k);
        #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    };

    // Walk the cost matrix
    double cost = 0;
    int l_i = y1.size();
    int l_j = y1.size();
    double mi, mj, mij;
    while(l_i > 0 && l_j > 0)
    {
        //cost += cost_matrix[(l_i)*mat_l + l_j];
        cost += cost_matrix[ij2k_rci(mat_l, l_i, l_j)];
        if (l_i > 0) {
            //mi = cost_matrix[(l_i-1)*mat_l + l_j];
            mi = cost_matrix[ij2k_rci(mat_l, l_i-1, l_j)];
        } else {
            mi = DBL_MAX;
        }
        if (l_j > 0) {
            //mj = cost_matrix[(l_i)*mat_l + l_j-1];
            mj = cost_matrix[ij2k_rci(mat_l, l_i, l_j-1)];
        } else {
            mj = DBL_MAX;
        }
        if (l_i > 0 && l_j > 0) {
            //mij = cost_matrix[(l_i-1)*mat_l + l_j-1];
            mij = cost_matrix[ij2k_rci(mat_l, l_i-1, l_j-1)];
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
    //cost += cost_matrix[(l_i)*mat_l + l_j];
    cost += cost_matrix[ij2k_rci(mat_l, l_i, l_j)];

    free(cost_matrix);
    return cost;
};


void __DTW_ThreadSingleBlock(ClassDataSingle y1, ClassDataSingle y2, double * cost_matrix, int mat_l, int row, int col, int row_w, int col_w)
{
    double cost, prev_min;
    for (int i = row; i < row + row_w; i++)
    {
        for (int j = col; j < col + col_w; j++)
        {
            cost = abs(y1[i-1] - y2[j-1]);
            // Take the last min from the window
            prev_min = min(cost_matrix[(i-1)*mat_l + j], cost_matrix[(i)*mat_l + j-1]);
            prev_min = min(prev_min, cost_matrix[(i-1)*mat_l + j-1]);
            cost_matrix[(i)*mat_l + j] = cost + prev_min;
        }
    }
};


double DynamicTimeWarping_ThreadTiled(ClassDataSingle y1, ClassDataSingle y2, unsigned int maxThreads)
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
    if (y1.size() < maxThreads)
    {
        maxThreads = y1.size();
    }
    unsigned int tileSize = y1.size() / maxThreads;

    #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    std::vector<std::thread> threads(maxThreads);
    #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)

    // Upper triangular cost calculations
    for (int k = 1; k <= maxThreads - 1; k++)
    {
        for (int n = 0; n < k; n++)
        {
            #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            threads[i] = std::thread(__DTW_ThreadSingleBlock, y1, y2, cost_matrix, mat_l, (k-n-1)*tileSize, n*tileSize, tileSize, tileSize);
            //threads[i] = ti0;
            #else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            __DTW_ThreadSingleBlock(y1, y2, cost_matrix, mat_l, (k-n-1)*tileSize, n*tileSize, tileSize, tileSize);
            #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        }

        #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        for (int n = 0; n < k; n++)
        {
            threads[i].join();
        }
        #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    }

    int Ilast = y1.size() - (maxThreads-1)*tileSize;
    int Jlast = y2.size() - (maxThreads-1)*tileSize;


    #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    threads[0] = std::thread(__DTW_ThreadSingleBlock, y1, y2, cost_matrix, mat_l, 0, (maxThreads-1)*tileSize, Ilast, tileSize);
    //threads[0] = ti1;
    for (int n = 1; n < maxThreads-1; n++)
    {
        threads[i] = std::thread(__DTW_ThreadSingleBlock, y1, y2, cost_matrix, mat_l, (maxThreads-n-1)*tileSize, n*tileSize, tileSize, tileSize);
        //threads[i] = ti2;
    }
    threads[maxThreads-1] = std::thread(__DTW_ThreadSingleBlock, y1, y2, cost_matrix, mat_l, 0, (maxThreads-1)*tileSize, tileSize, Jlast);
    //threads[maxThreads-1] = ti3;
    for (int n = 0; n < maxThreads; n++)
    {
        threads[i].join();
    }
    #else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    __DTW_ThreadSingleBlock(y1, y2, cost_matrix, mat_l, 0, (maxThreads-1)*tileSize, Ilast, tileSize); // TODO: thread this
    for (int n = 1; n < maxThreads-1; n++)
    {
        __DTW_ThreadSingleBlock(y1, y2, cost_matrix, mat_l, (maxThreads-n-1)*tileSize, n*tileSize, tileSize, tileSize); // TODO: thread this
    }
    __DTW_ThreadSingleBlock(y1, y2, cost_matrix, mat_l, 0, (maxThreads-1)*tileSize, tileSize, Jlast); // TODO: thread this
    #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)

    for (int k = maxThreads; k > 0; k--)
    {
        for (int n = 0; n < k; n++)
        {
            #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            threads[i] = std::thread(__DTW_ThreadSingleBlock, y1, y2, cost_matrix, mat_l, (k-n-1)*tileSize, n*tileSize, n == 0 ? Ilast : tileSize, n == k - 1 ? Jlast : tileSize);
            //threads[i] = ti4;
            #else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
            __DTW_ThreadSingleBlock(y1, y2, cost_matrix, mat_l, (k-n-1)*tileSize, n*tileSize, n == 0 ? Ilast : tileSize, n == k - 1 ? Jlast : tileSize);
            #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        }

        #if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
        for (int n = 0; n < k; n++)
        {
            threads[i].join();
        }
        #endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    }


    // Walk the cost matrix
    double cost = 0;
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


double DynamicTimeWarping_ThreadTiled(ClassDataSingle y1, ClassDataSingle y2)
{
    return DynamicTimeWarping_ThreadTiled(y1, y2, 4);
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
#endif /* DTW_MULTITHREADED_H_ */
