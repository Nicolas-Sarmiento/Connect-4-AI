cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(connect4 src/main.cpp)
