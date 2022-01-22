cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(replace-multiple-words VERSION 1.0)

add_executable(replace-multiple-words main.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)