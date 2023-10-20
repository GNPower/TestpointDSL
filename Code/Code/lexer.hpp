/*******************************************************************************
*   Hyperdimensional Computing Library
*
*   @file       lexer.hpp
*   @desc       Lexer implementation for HIL custom DSL
*   @author     Graham N. Power
*   @date       2023-10-19
*   @version    0.1.0 (Major.Minor.Patch)
*
*	<pre>
*
*   Revision History:
*
*	Ver   	Who    	Date   		Changes
*	----- 	---- 	-------- 	-------------------------------------------------------
*	0.1.0 	gnp   	2023-10-19 	Bare bones implementation based off of LLVM tutorial
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
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#ifndef HIL_DSL_LEXER_H_
#define HIL_DSL_LEXER_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <string>


/*******************************************************************************
*   Preprocessor Macros
*******************************************************************************/


/*******************************************************************************
*   Datatype Definitions
*******************************************************************************/
// The lexer returns tokens [0-25] if it is an unknown character, otherwise one of these known things.
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal;               // Filled in if tok_number

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
// gettok - Return the next token from standard input
static int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace
    while (isspace(LastChar))
        LastChar = getchar();

    // Recognize keyworks like "def"
    if (isalpha(LastChar))
    { // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    // Recognize numbers with or without decimals
    if (isdigit(LastChar) || LastChar == '.')
    { // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getchar();
        }
        while(isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    // Recognize code comments
    if (LastChar == '#')
    {
        // Comment until end of line
        do
        {
            LastChar = getchar();
        } 
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        
        if (LastChar != EOF)
            return gettok();
    }

    // The input doesn't match any of the above known cases. So if its not the EOF, return directly
    if (LastChar == EOF)
        return tok_eof;

    // Just return the character as its ascii value
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
};

/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* HIL_DSL_LEXER_H_ */
