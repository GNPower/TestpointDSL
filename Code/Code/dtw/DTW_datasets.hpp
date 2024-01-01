/*******************************************************************************
*   ECE-718:  Compiler Design for High Performance Computing, Final Project
*
*   @file       DTW_datasets.hpp
*   @desc       Header library for DTW data structures compatible with the UCR Archive 2018
*   @author     Graham N. Power
*   @date       2023-12-12
*   @version    1.0.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	1.0.0 	gnp   	2023-12-12 	Moved datatype defenitions from archive_loader.hpp to dedicated file
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
#ifndef DTW_DATASETS_H_
#define DTW_DATASETS_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <vector>
#include <cfloat>
#include <stdio.h>
#include <math.h>


/*******************************************************************************
*   Preprocessor Macros
*******************************************************************************/
template <typename T>
T min(T a, T b)
{
    if (a < b)
        return a;
    return b;
}

template <typename T>
T max(T a, T b)
{
    if (a > b)
        return a;
    return b;
}


/*******************************************************************************
*   Datatype Definitions
*******************************************************************************/
typedef std::vector<int> ClassLabels;
typedef std::vector<double> ClassDataSingle;
typedef std::vector<ClassDataSingle> ClassData;
typedef double (Classification_Function)(ClassDataSingle comparisonData, ClassDataSingle objectToClassify);

typedef struct {
    ClassLabels ClassLabels_Train;
    ClassLabels ClassLabels_Test;
    ClassData ClassData_Train;
    ClassData ClassData_Test;
} UCR_Dataset_t;


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
void PrintMatrix(double * mat, unsigned int ylength)
{
    unsigned int i, j;
    for (i = 0; i < ylength; i++)
    {
        for (j = 0; j < ylength; j++)
        {
            if ((DBL_MAX - mat[i*ylength + j]) < 10000 )
            {
                printf("DBL_MAX  ");
            }
            else
            {
                printf("%10.6f  ", mat[i*ylength + j]);
            }            
        }
        printf("\n");
    }
    printf("\n\n\n\n");
};


template <typename T>
void PrintVector(std::vector<T> vec)
{
    unsigned int i;
    for (i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i] << ", ";       
    }
    std::cout << std::endl;
};


double Classification_1NNEuclid(ClassDataSingle comparisonData, ClassDataSingle objectToClassify)
{
        double distance = 0;
        for (int j = 0; j < comparisonData.size(); j++)
        {
            distance += pow(comparisonData[j] - objectToClassify[j], 2);
        }
        distance = sqrt(distance);

        return distance;
};


int Classify_ClassData(ClassData ClassData_Train, ClassLabels ClassLabels_Train, ClassDataSingle objectToClassify, Classification_Function FunctionToCall)
{
    double bestSoFar = DBL_MAX;
    int predictedClass = 0;

    //std::cout << "Distances: "; 
    for (int i = 0; i < ClassLabels_Train.size(); i++)
    {
        ClassDataSingle comparisonData = ClassData_Train[i];

        double distance = FunctionToCall(comparisonData, objectToClassify);

        //std::cout << distance << ", ";

        if (distance < bestSoFar)
        {
            predictedClass = ClassLabels_Train[i];
            bestSoFar = distance;
        }
    }
    //std::cout << std::endl;

    return predictedClass;
};


int Classify_ClassData(ClassData ClassData_Train, ClassLabels ClassLabels_Train, ClassDataSingle objectToClassify, Classification_Function FunctionToCall, double lowerBound)
{
    double bestSoFar = DBL_MAX;
    int predictedClass = 0;

    //std::cout << "Distances: "; 
    for (int i = 0; i < ClassLabels_Train.size(); i++)
    {
        ClassDataSingle comparisonData = ClassData_Train[i];

        double distance = FunctionToCall(comparisonData, objectToClassify);

        //std::cout << distance << ", ";

        if (distance < bestSoFar)
        {
            predictedClass = ClassLabels_Train[i];
            bestSoFar = distance;
        }

        if (bestSoFar <= lowerBound)
        {
            return predictedClass;
        }
    }
    //std::cout << std::endl;

    return predictedClass;
};


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* DTW_DATASETS_H_ */