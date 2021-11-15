cmake_minimum_required(VERSION 3.15)
project(prefer_consts_enums_inlines)

set(CMAKE_CXX_STANDARD 14)

add_executable(disadvantages_of_macros 01_disadvantages_of_macros.cpp)
add_executable(enum_hack 02_enum_hack.cpp)
