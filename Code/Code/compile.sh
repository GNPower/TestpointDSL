# Compile main:
clang++ -o testpointllvm `llvm-config --cxxflags --ldflags --system-libs --libs core` TestpointLLVM.cpp

# Run main:
./testpointllvm

# Execute generated IR
lli ./out.ll

# Print result:
echo $?
printf "\n"