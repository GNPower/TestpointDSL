/*******************************************************************************
*   Hyperdimensional Computing Library
*
*   @file       hil_dsp.cpp
*   @desc       Compiler for HIL custom DSL
*   @author     Graham N. Power
*   @date      	2023-10-19
*   @version    0.1.0 (Major.Minor.Patch)
*
*	<pre>
*
*	Detailed description of driver goes here.
*
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	0.1.0 	gnp   	2023-01-19 	Bare bones implementation based off of LLVM tutorial
*                               https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html
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
*   Includes
*******************************************************************************/
#include <stdio.h>

#include "parser.hpp"


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
int main() {
    // Install standard binary operators.
    PopulateBinopPrecedence();

    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();

    // Make the module, which holds all the code.
    InitializeModule();

    // Run the main "interpreter loop" now.
    ParserMainLoop();

    // Print out all of the generated code.
    TheModule->print(errs(), nullptr);

    return 0;
};
