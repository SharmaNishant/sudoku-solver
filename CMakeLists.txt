cmake_minimum_required(VERSION 3.3)
project(AISoft_HW1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SOURCE_FILES main.cpp Sudoku.cpp include/Sudoku.h include/GlobalFunctions.h GlobalFunctions.cpp GlobalFunctions.cpp include/GlobalFunctions.h)
add_executable(Sudoku_Solver ${SOURCE_FILES})
include_directories( include )