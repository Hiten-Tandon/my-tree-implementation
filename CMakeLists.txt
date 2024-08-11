cmake_minimum_required(VERSION 3.5)

project(
  tree
  VERSION 0.1
  LANGUAGES CXX)

add_executable(tree src/tree.cpp)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/out")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
