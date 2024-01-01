/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       archive_loader.hpp
*   @desc       Header library for loading data from the UCR Archive 2018
*   @author     Graham N. Power
*   @date       2023-12-10
*   @version    1.0.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-10 	Functions for loading data from the UCR Archive 2018
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
#ifndef DTW_ARCHIVE_LOADER_H_
#define DTW_ARCHIVE_LOADER_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <float.h>
#include <math.h>

#include "dtw/DTW_datasets.hpp"


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
void PrintDatasetContents(UCR_Dataset_t * dataset)
{
    std::cout << "The train label vector elements are : " << std::endl;
    for (int i = 0; i < dataset->ClassLabels_Train.size(); i++)
    {
        std::cout << dataset->ClassLabels_Train[i] << ", ";
    }
    std::cout << std::endl;
};


int LoadDatasetUCR(UCR_Dataset_t * dataset, std::string train_file, std::string test_file)
{
    std::ifstream data;
    std::string line;
    int label;
    double value;

    // Empty dataset structure
    dataset->ClassData_Test.clear();
    dataset->ClassData_Train.clear();
    dataset->ClassLabels_Test.clear();
    dataset->ClassData_Train.clear();

    // Load the train data
    data.open(train_file);
    if (!data)
    {
        // File not found, oops
        return -1;
    }

    while (std::getline(data, line))
    {
        std::istringstream ss(line);
        
        ss >> label;
        dataset->ClassLabels_Train.push_back(label);

        dataset->ClassData_Train.push_back({});
        
        while(ss >> value)
        {
            dataset->ClassData_Train.back().push_back(value);
        }
    }

    data.close();

    // Load the test data
    data.open(test_file);
    if (!data)
    {
        // File not found, oops
        return -1;
    }

    while (std::getline(data, line))
    {
        std::istringstream ss(line);

        ss >> label;
        dataset->ClassLabels_Test.push_back(label);

        dataset->ClassData_Test.push_back({});

        while(ss >> value)
        {
            dataset->ClassData_Test.back().push_back(value);
        }
    }
    
    data.close();

    return 0;
};


double RunDatasetTests(UCR_Dataset_t * dataset, Classification_Function FunctionToCall)
{
    int correct = 0;

    std::cout << "Running " << dataset->ClassLabels_Test.size() << " Dataset Tests" << std::endl;
    for (int i = 0; i < dataset->ClassLabels_Test.size(); i++)
    {
	std::cout << i << std::endl;
        std::vector<double> objectToClassify = dataset->ClassData_Test[i];
        int objectClass = dataset->ClassLabels_Test[i];

        int predictedClass = Classify_ClassData(dataset->ClassData_Train, dataset->ClassLabels_Train, objectToClassify, FunctionToCall);

        if (predictedClass == objectClass)
        {
            correct++;
        }
        // std::cout << i << " out of " << dataset->ClassLabels_Test.size() << " done" << std::endl;
        std::cout << ".";
    }
    std::cout << "\nTest Run Complete!" << std::endl;

    double errorRate = (double)(dataset->ClassLabels_Test.size() - correct) / (double)(dataset->ClassLabels_Test.size());

    std::cout << "The dataset you tested has " << dataset->ClassLabels_Train.size() << " classes" << std::endl;
    std::cout << "The training set is of size " << dataset->ClassData_Train.size() << ", and the test set is of size " << dataset->ClassData_Test.size() << "." << std::endl;
    std::cout << "The time series are of length " << dataset->ClassData_Train[0].size() << std::endl;
    std::cout << correct << " of " << dataset->ClassLabels_Test.size() << " classifications were correct" << std::endl;
    std::cout << "The error rate was " << errorRate << std::endl;

    return errorRate;
};


double RunDatasetTests(UCR_Dataset_t * dataset, Classification_Function FunctionToCall, double lowerBound)
{
    int correct = 0;

    for (int i = 0; i < dataset->ClassLabels_Test.size(); i++)
    {
        std::vector<double> objectToClassify = dataset->ClassData_Test[i];
        int objectClass = dataset->ClassLabels_Test[i];

        int predictedClass = Classify_ClassData(dataset->ClassData_Train, dataset->ClassLabels_Train, objectToClassify, FunctionToCall, lowerBound);

        if (predictedClass == objectClass)
        {
            correct++;
        }
        std::cout << i << " out of " << dataset->ClassLabels_Test.size() << " done" << std::endl;
    }

    double errorRate = (double)(dataset->ClassLabels_Test.size() - correct) / (double)(dataset->ClassLabels_Test.size());

    std::cout << "The dataset you tested has " << dataset->ClassLabels_Train.size() << " classes" << std::endl;
    std::cout << "The training set is of size " << dataset->ClassData_Train.size() << ", and the test set is of size " << dataset->ClassData_Test.size() << "." << std::endl;
    std::cout << "The time series are of length " << dataset->ClassData_Train[0].size() << std::endl;
    std::cout << correct << " of " << dataset->ClassLabels_Test.size() << " classifications were correct" << std::endl;
    std::cout << "The error rate was " << errorRate << std::endl;

    return errorRate;
};


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_ARCHIVE_LOADER_H_ */
