/*******************************************************************************
*   Hyperdimensional Computing Library
*
*   @file       parser.hpp
*   @desc       Parser implementation for HIL custom DSL
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
#ifndef HIL_DSL_PARSER_H_
#define HIL_DSL_PARSER_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include "lexer.hpp"
#include "ast.hpp"
#include "codegen.hpp"


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
static std::unique_ptr<ExprAST> ParseExpression();
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);


/*******************************************************************************
*   Global Variables
*******************************************************************************/
// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
// token the parser is looking at.  getNextToken reads another token from the
// lexer and updates CurTok with its results.
static int CurTok;

// BinopPrecedence - This holds the precedence for each binary operator that is
// defined.
static std::map<char, int> BinopPrecedence;


/*******************************************************************************
*   Functions / Classes
*******************************************************************************/


/*
2.3 PARSER BASICS
*/

// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
// token the parser is looking at.  getNextToken reads another token from the
// lexer and updates CurTok with its results.
static int getNextToken()
{
    return CurTok = gettok();
};


// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str)
{
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
};

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
    LogError(Str);
    return nullptr;
};


/*
2.4 BASIC EXPRESSION PARSING
*/

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // consume the number
    return std::move(Result);
};

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ).
    return V;
};

// identifierexpr
//   ::= identifier
//   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() 
{
    std::string IdName = IdentifierStr;

    getNextToken();  // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return std::make_unique<VariableExprAST>(IdName);

    // Call.
    getNextToken();  // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')') 
    {
        while (true) 
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
};

// primary
//   ::= identifierexpr
//   ::= numberexpr
//   ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() 
{
    switch (CurTok) 
    {
        default:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
};


/*
2.5 BINARY EXPRESSION PARSING
*/

static void PopulateBinopPrecedence()
{
    // Install standard binary operators.
    // 1 is lowest precedence.
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;  // highest.
};

// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
    if (!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) return -1;
    return TokPrec;
};

// expression
//   ::= primary binoprhs
//
static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/// binoprhs
///   ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) 
{
    // If this is a binop, find its precedence.
    while (true) 
    {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) 
        {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) return nullptr;
        }

        // Merge LHS/RHS.
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
};


/*
2.6 PARSING THE REST
*/

// prototype
//   ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success.
    getNextToken();  // eat ')'.

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
};

// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken();  // eat def.
    auto Proto = ParsePrototype();
    if (!Proto) return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
};

// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken();  // eat extern.
    return ParsePrototype();
};

// toplevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
    if (auto E = ParseExpression()) 
    {
        // Make an anonymous proto.
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
};


/*
2.7 THE DRIVER
*/

// static void HandleDefinition() {
//     if (ParseDefinition()) 
//     {
//         fprintf(stderr, "Parsed a function definition.\n");
//     } 
//     else 
//     {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// };

// static void HandleExtern() {
//     if (ParseExtern()) 
//     {
//         fprintf(stderr, "Parsed an extern\n");
//     } 
//     else 
//     {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// };

// static void HandleTopLevelExpression() {
//     // Evaluate a top-level expression into an anonymous function.
//     if (ParseTopLevelExpr()) 
//     {
//         fprintf(stderr, "Parsed a top-level expr\n");
//     } 
//     else 
//     {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// };

static void InitializeModule() {
  // Open a new context and module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

static void HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read function definition:");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      fprintf(stderr, "Read extern: ");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = ParseTopLevelExpr()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read top-level expression:");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");

      // Remove the anonymous expression.
      FnIR->eraseFromParent();
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

// top ::= definition | external | expression | ';'
static void ParserMainLoop() {
    while (true) 
    {
        fprintf(stderr, "ready> ");
        switch (CurTok) 
        {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
};


/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* HIL_DSL_PARSER_H_ */
