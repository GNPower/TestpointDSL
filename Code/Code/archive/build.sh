#!/bin/bash

CONFIG=${1:-Debug}

rm -rf build
mkdir build
cd build/
cmake .. -DCMAKE_BUILD_TYPE=Release
# cmake --build .
cmake --build . --config $CONFIG