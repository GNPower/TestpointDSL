CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

# Set the project name
project(hil_dsl)

find_package( OpenMP REQUIRED)
if(OpenMP_FOUND)
    add_compile_definitions(OPENMP_ENABLED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

endif()

add_executable(hil_dsl
        hil_dsl.cpp)
target_link_libraries(hil_dsl
        OpenMP::OpenMP_CXX
)