CMAKE_MINIMUM_REQUIRED(VERSION 3.16)


# Set the project name
project(plotgen)

set(CMAKE_CXX_FLAGS -pthread)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/plotGen
        ${CMAKE_CURRENT_SOURCE_DIR}/dtw
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomPulseDataset
)

add_executable(plotgen
        generateAllPlotData.cpp)
