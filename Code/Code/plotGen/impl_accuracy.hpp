/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       impl_accuracy.hpp
*   @desc       Header library for implementation accuracy plot data generation
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
#ifndef DTW_IMPL_ACCURACY_H_
#define DTW_IMPL_ACCURACY_H_


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
#define ACCURACY_NUM_IMPLEMENTATIONS     11
#define ACCURACY_OUTPUT_FILE_NAME        "plotGen/accuracy_results.txt"


/*******************************************************************************
*   Datatype Definitions
*******************************************************************************/
typedef double (*DTW_accuracy)(ClassDataSingle y1, ClassDataSingle y2);

typedef struct {
    UCR_Dataset_t dataset;
    DTW_accuracy funcs[ACCURACY_NUM_IMPLEMENTATIONS];
    std::string funcNames[ACCURACY_NUM_IMPLEMENTATIONS];
    double results[ACCURACY_NUM_IMPLEMENTATIONS];
} Accuracy_t;

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
void CompareImplementations_Accuracy()
{
    // CALCULATE ACCURACIES
    std::cout << "Calculating Accuracies..." << std::endl;
    Accuracy_t accuracies;

    // Load the UCR dataset
    std::cout << "Loading UCR Dataset..." << std::endl;
    LoadDatasetUCR(&accuracies.dataset, "UCRArchive_2018/SyntheticControl/SyntheticControl_TRAIN.tsv", "UCRArchive_2018/SyntheticControl/SyntheticControl_TEST.tsv");
    std::cout << "Dataset Load Complete!" << std::endl;

    std::cout << "Load funcs to accuracy check..." << std::endl;
    // Set the functions to time
    accuracies.funcs[0] = DynamicTimeWarping_Basic;
    accuracies.funcNames[0] = "Original";
    accuracies.funcs[1] = DynamicTimeWarping_SwappedLoops;
    accuracies.funcNames[1] = "Swapped Loops";
    accuracies.funcs[2] = DynamicTimeWarping_ReducedBitWidths;
    accuracies.funcNames[2] = "Small Bitwidth";
    accuracies.funcs[3] = DynamicTimeWarping_PartialCostInit;
    accuracies.funcNames[3] = "Partial Init";
    accuracies.funcs[4] = DynamicTimeWarping_ThreadRowCol;
    accuracies.funcNames[4] = "Thread Row/Col";
    accuracies.funcs[5] = DynamicTimeWarping_ThreadRowColInterleaved;
    accuracies.funcNames[5] = "Interleave";
    accuracies.funcs[6] = DynamicTimeWarping_WarpConstrained;
    accuracies.funcNames[6] = "Limit Warping";
    accuracies.funcs[7] = DynamicTimeWarping_Memoization;
    accuracies.funcNames[7] = "Memoization";
    accuracies.funcs[8] = DynamicTimeWarping_EarlyAbandoned;
    accuracies.funcNames[8] = "Early Abandon";
    accuracies.funcs[9] = DynamicTimeWarping_Pruned;
    accuracies.funcNames[9] = "Pruned";
    //accuracies.funcs[10] = DynamicTimeWarping_ThreadTiled;
    //accuracies.funcNames[10] = "Thread Tiled";
    accuracies.funcs[10] = Classification_1NNEuclid;
    accuracies.funcNames[10] = "1NN Euclid (Not DTW)";

    // Calculate accuracies
    for (int j = 0; j < ACCURACY_NUM_IMPLEMENTATIONS; j++)
    {
        std::cout << "\t\tImpl: " << j+1 << " of " << ACCURACY_NUM_IMPLEMENTATIONS << std::endl;

        accuracies.results[j] = RunDatasetTests(&accuracies.dataset, accuracies.funcs[j]);
    }

    std::cout << "\n\n\n";

    std::ostringstream results_csv;

    std::cout << "Accuracy Results:\n\n";

    // Print Header
    for (int i = 0; i < ACCURACY_NUM_IMPLEMENTATIONS; i++)
    {
        std::cout << accuracies.funcNames[i] << ", ";
        results_csv << accuracies.funcNames[i] << ", ";
    }
    std::cout << "\n";
    results_csv << "\n";

    // Print Data
    for (int i = 0; i < ACCURACY_NUM_IMPLEMENTATIONS; i++)
    {
        std::cout << accuracies.results[i] << ", ";
        results_csv << accuracies.results[i] << ", ";
    }
    std::cout << "\n\n\n" << std::endl;

    std::ofstream resultsFile(ACCURACY_OUTPUT_FILE_NAME);
    resultsFile << results_csv.str();
    resultsFile.close();
};


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_IMPL_ACCURACY_H_ */