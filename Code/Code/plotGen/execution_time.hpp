/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       execution_time.hpp
*   @desc       Header library for execution time plot data generation
*   @author     Graham N. Power
*   @date       2023-12-19
*   @version    1.1.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-19 	Initial implementation
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
#ifndef DTW_EXECUTION_TIME_H_
#define DTW_EXECUTION_TIME_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <iostream>

#include "../archive_loader.hpp"
#include "../dtw/DTW_datasets.hpp"
#include "../dtw/DTW_basic.hpp"
#include "../dtw/DTW_early_abandon.hpp"
#include "../dtw/DTW_restructured.hpp"
#include "../dtw/DTW_threaded.hpp"


/*******************************************************************************
*   Preprocessor Macros
*******************************************************************************/
#define TIMING_NUM_IMPLEMENTATIONS     10
#define TIMING_MONTECARLO_RUNS         10
#define TIMING_NUM_VECSIZES            14 // 12 => 4096
#define TIMING_OUTPUT_FILE_NAME        "plotGen/timing_results.txt"


/*******************************************************************************
*   Datatype Definitions
*******************************************************************************/
typedef double (*DTW_timing)(ClassDataSingle y1, ClassDataSingle y2);

typedef struct {
    unsigned int vecSizes[TIMING_NUM_VECSIZES];
    DTW_timing funcs[TIMING_NUM_IMPLEMENTATIONS];
    std::string funcNames[TIMING_NUM_IMPLEMENTATIONS];
    double results[TIMING_NUM_IMPLEMENTATIONS][TIMING_NUM_VECSIZES];
} ExecutionTime_t;


/*******************************************************************************
*   Constant Definitions
*******************************************************************************/


/*******************************************************************************
*   Function Protoypes
*******************************************************************************/
void TimeDTWImplementations(ExecutionTime_t * results);
void PrintDTWResults(ExecutionTime_t * results);


/*******************************************************************************
*   Global Variables
*******************************************************************************/


/*******************************************************************************
*   Functions
*******************************************************************************/
void CompareImplementations_ExecutionTime()
{
    // CALCULATE EXECUTION TIMES
    std::cout << "Calculating Execution Times..." << std::endl;
    ExecutionTime_t times;

    // Set vecsizes based on 2**i
    std::cout << "Load VecSizes..." << std::endl;
    unsigned int i;
    for (i = 0; i < TIMING_NUM_VECSIZES; i++)
    {
        times.vecSizes[i] = pow(2, i+1);
        std::cout << times.vecSizes[i] << ", ";
    }
    std::cout << std::endl;

    std::cout << "Load funcs to time..." << std::endl;
    // Set the functions to time
    times.funcs[0] = DynamicTimeWarping_Basic;
    times.funcNames[0] = "Original";
    times.funcs[1] = DynamicTimeWarping_SwappedLoops;
    times.funcNames[1] = "Swapped Loops";
    times.funcs[2] = DynamicTimeWarping_ReducedBitWidths;
    times.funcNames[2] = "Small Bitwidth";
    times.funcs[3] = DynamicTimeWarping_PartialCostInit;
    times.funcNames[3] = "Partial Init";
    times.funcs[4] = DynamicTimeWarping_ThreadRowCol;
    times.funcNames[4] = "Thread Row/Col";
    times.funcs[5] = DynamicTimeWarping_ThreadRowColInterleaved;
    times.funcNames[5] = "Thread Row/Col Interleave";
    times.funcs[6] = DynamicTimeWarping_WarpConstrained;
    times.funcNames[6] = "Limit Warping";
    times.funcs[7] = DynamicTimeWarping_Memoization;
    times.funcNames[7] = "Memoization";
    times.funcs[8] = DynamicTimeWarping_EarlyAbandoned;
    times.funcNames[8] = "Early Abandon";
    times.funcs[9] = DynamicTimeWarping_Pruned;
    times.funcNames[9] = "Pruned";
    //times.funcs[10] = DynamicTimeWarping_ThreadTiled;
    //times.funcNames[10] = "Thread Tiled";

    // Call the timing code
    TimeDTWImplementations(&times);

    // Print the results
    PrintDTWResults(&times);
};


double drand()
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
    std::cout << "Beggining DTW Timing..." << std::endl;
    unsigned int i, j, k;

    // // Create random input vectors
    std::cout << "Generating Random Vectors (" << results->vecSizes[0] << " -> " << results->vecSizes[TIMING_NUM_VECSIZES-1] << ")..." << std::endl;
    std::vector<ClassDataSingle> veca;
    std::vector<ClassDataSingle> vecb;

    for (int i = 0; i < TIMING_NUM_VECSIZES; i++)
    {
        std::cout << "\t" << results->vecSizes[i] << std::endl;
        veca.push_back({});
        vecb.push_back({});
        for (int j = 0; j < results->vecSizes[i]; j++)
        {
            veca[i].push_back(drand());
            vecb[i].push_back(drand());
        }
    }
    std::cout << "Random Vector Generation Complete!" << std::endl;

    // Get timing results
    clock_t before[TIMING_MONTECARLO_RUNS];
    clock_t after[TIMING_MONTECARLO_RUNS];
    for (i = 0; i < TIMING_NUM_VECSIZES; i++)
    {
        std::cout << "\tVecsize: " << i+1 << " of " << TIMING_NUM_VECSIZES << std::endl;
        for (j = 0; j < TIMING_NUM_IMPLEMENTATIONS; j++)
        {
            std::cout << "\t\tImpl: " << j+1 << " of " << TIMING_NUM_IMPLEMENTATIONS << std::endl;
            for (k = 0; k <  TIMING_MONTECARLO_RUNS; k++)
            {
                std::cout << "\t\t\tMonteCarlo: " << k+1 << " of " << TIMING_MONTECARLO_RUNS << std::endl;
                before[k] = clock();
                results->funcs[j](veca[i], vecb[i]);
                after[k] = clock();
                // printf("\t\t\t\tBefore: %d  After: %d\n", before[k], after[k]);
            }
            results->results[j][i] = gmean(before, after, TIMING_MONTECARLO_RUNS);
            // printf("\t\t\tGMean2: %f\n", results->results[j][i]);
        }
    }

    printf("\n\n\n");
};


void PrintDTWResults(ExecutionTime_t * results)
{
    std::ostringstream results_csv;

    std::cout << "Timing Results:\n\n";
    unsigned int i, j;

    std::cout << "Clocks Per Second: " << CLOCKS_PER_SEC << "\n\n";
    results_csv << "Clocks Per Second: " << CLOCKS_PER_SEC << "\n";

    // Print Header
    std::cout << "Vector Size, ";
    results_csv << "Vector Size, ";
    for (i = 0; i < TIMING_NUM_IMPLEMENTATIONS; i++)
    {
        std::cout << results->funcNames[i] << ", ";
        results_csv << results->funcNames[i] << ", ";
    }
    std::cout << "\n";
    results_csv << "\n";

    // Print Data
    for (i = 0; i < TIMING_NUM_VECSIZES; i++)
    {
        std::cout << results->vecSizes[i] << ", ";
        results_csv << results->vecSizes[i] << ", ";
        for (j = 0; j < TIMING_NUM_IMPLEMENTATIONS; j++)
        {
            std::cout << results->results[j][i] << ", ";
            results_csv << std::fixed << results->results[j][i] << ", ";
        }
        std::cout << "\n";
        results_csv << "\n";
    }
    std::cout << "\n\n\n" << std::endl;

    std::ofstream resultsFile(TIMING_OUTPUT_FILE_NAME);
    resultsFile << results_csv.str();
    resultsFile.close();
};


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_EXECUTION_TIME_H_ */
