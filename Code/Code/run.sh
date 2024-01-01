#!/bin/bash

# CONFIG=${1:-Release}
CNAME=${2:-plotgen}

echo -e "Running C Code:\n\n"


# Windows Command
# ./build/Debug/plotgen.exe
# Niagara Command
./build/plotgen

echo -e "Running Python Plot Gen:\n\n"

python ./generateAllPlots.py

echo -e "Running Python DSL Examples:\n\n"

python ./transpilerDSL/testpointDSL.py -r false -f transpilerDSL/test
python ./transpilerDSL/testpointDSL.py -r true -f transpilerDSL/test2
