#include <iostream>

#include "CustomPulseDataset/generate_pulses.hpp"
#include "plotGen/execution_time.hpp"
#include "plotGen/impl_accuracy.hpp"

#include "archive_loader.hpp"
#include "dtw/DTW_datasets.hpp"
#include "dtw/DTW_basic.hpp"
#include "dtw/DTW_early_abandon.hpp"
#include "dtw/DTW_restructured.hpp"
#include "dtw/DTW_threaded.hpp"


int main(int argc, char *argv[]) {

    // Generate a demo pulse dataset
    generatePulseDataset(
        100, // length
        1, // stride
        "CustomPulseDataset/ExpPulse_Demo_TRAIN.tsv", // trainFileName
        "CustomPulseDataset/ExpPulse_Demo_TEST.tsv" // testFileName
    );
    // Verify this custom dataset still performs better with DTW
    UCR_Dataset_t dataset;
    LoadDatasetUCR(
        &dataset, 
        "CustomPulseDataset/ExpPulse_Demo_TRAIN.tsv", 
        "CustomPulseDataset/ExpPulse_Demo_TEST.tsv"
    );
    double errorRateEuclid = RunDatasetTests(&dataset, Classification_1NNEuclid);
    double errorRateDTW = RunDatasetTests(&dataset, DynamicTimeWarping_Basic);
    std::cout << "\n\n\nCustom Dataset Error Rates:\n\tEuclid: " << errorRateEuclid << "\n\tDTW: " << errorRateDTW << "\n\n" << std::endl;

    // CompareImplementations_ExecutionTime();

    CompareImplementations_Accuracy();

    return 0;
};
