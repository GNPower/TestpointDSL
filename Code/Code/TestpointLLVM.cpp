
#define LLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING 1

#include <string>

#include "TestpointLLVM.hpp"

int main(int argc, char const *argv[])
{
    std::string program = R"(

        (printf "Hello, World! %d" 42)

    )";

    TestpointLLVM vm;

    vm.exec(program);

    return 0;
};