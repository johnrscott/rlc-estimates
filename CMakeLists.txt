cmake_minimum_required(VERSION 3.10)

# set the project name
project(rlc VERSION 0.1 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#find_package(OpenMP REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    if (MSVC)
        #set(OpenMP_CXX_FLAGS, "-openmp:llvm") # Experimental version of openmp
        #set(OpenMP_CXX_FLAGS "")
        set(CMAKE_CXX_FLAGS "/EHsc /openmp:llvm")  
    else()
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -pg -O2 -fopenmp")  
    endif (MSVC)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -pg -O2 -fmax-errors=3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -pg -O2 -fmax-errors=3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  #message( FATAL_ERROR "Microsoft Visual Studio is not supported yet." )  
  #set(CMAKE_CXX_FLAGS "/EHsc /std:c++11")
endif()

# Example
add_executable(rlc rlc-estimate.cpp estimation.cpp simulation.cpp file.cpp spectral.cpp fft.cpp)
