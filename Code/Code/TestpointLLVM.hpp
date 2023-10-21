/*******************************************************************************
*   Hyperdimensional Computing Library
*
*   @file       ast.hpp
*   @desc       Abstract Syntax Tree implementation for HIL custom DSL
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
#ifndef TESTPOINT_DSL_LLVM_H_
#define TESTPOINT_DSL_LLVM_H_


/*******************************************************************************
*   Includes
*******************************************************************************/
#include <string>
#include <memory>
#include <iostream>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"


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
*   Functions / Classes
*******************************************************************************/
class TestpointLLVM {
    public:
        TestpointLLVM() 
        { 
            moduleInit(); 
            setupExternFunctions();
        };

        // Executes a program
        void exec(const std::string &program)
        {
            // 1. Parse the program
            // auto ast = parser->parser(program);

            // 2. Compile to LLVM IR
            compile();

            // Print generated code.
            module->print(llvm::outs(), nullptr);
            std::cout << std::endl << std::endl;

            // 3. Same module IR to file
            saveModuleToFile("./out.ll");
        }

    private:
        llvm::Function *fn;
        std::unique_ptr<llvm::LLVMContext> ctx;
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder;

        void moduleInit() 
        {
            ctx = std::make_unique<llvm::LLVMContext>();
            module = std::make_unique<llvm::Module>("TestpointLLVM", *ctx);
            builder = std::make_unique<llvm::IRBuilder<>>(*ctx);
        };

        void saveModuleToFile(const std::string &fileName)
        {
            std::error_code errorCode;
            llvm::raw_fd_ostream outLL(fileName, errorCode);
            module->print(outLL, nullptr);
        };        

        void compile(/* TODO: ast*/) {
            // 1. Create main function
            fn = createFunction("main", llvm::FunctionType::get(/* return type */ builder->getInt32Ty(), /* vararg */ false));

            // 2. Compile main body
            gen(/* ast */);

            // auto i32Result = builder->CreateIntCast(result, builder->getInt32Ty(), true);

            builder->CreateRet(builder->getInt32(0));
        };

        llvm::Value* gen(/* exp */)
        {
            // return builder->getInt32(42);

            auto str = builder->CreateGlobalStringPtr("Hello World!\n");

            // Call to printf:
            auto printfFn = module->getFunction("printf");
            std::vector<llvm::Value*> args{str};

            return builder->CreateCall(printfFn, args);
        };

        void setupExternFunctions() {
            auto bytePtrTy = builder->getInt8Ty()->getPointerTo();

            module->getOrInsertFunction("printf", llvm::FunctionType::get(builder->getInt32Ty(), bytePtrTy, true));
        };

        llvm::Function* createFunction(const std::string &fnName, llvm::FunctionType *fnType)
        {
            // Function prototype might already be defined
            auto fn = module->getFunction(fnName);

            // If not, allocate the function
            if (fn == nullptr)
            {
                fn = createFunctionProto(fnName, fnType);
            }

            createFunctionBlock(fn);

            return fn;
        };

        llvm::Function* createFunctionProto(const std::string &fnName, llvm::FunctionType *fnType)
        {
            auto fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);

            verifyFunction(*fn);

            return fn;
        };

        void createFunctionBlock(llvm::Function *fn)
        {
            auto entry = createBB("entry", fn);
            builder->SetInsertPoint(entry);
        };

        llvm::BasicBlock* createBB(std::string name, llvm::Function* fn = nullptr)
        {
            return llvm::BasicBlock::Create(*ctx, name, fn);
        };
};

/*******************************************************************************
*   Prevent circular dependency
*   DO NOT REMOVE
*******************************************************************************/
#endif /* TESTPOINT_DSL_LLVM_H_ */
