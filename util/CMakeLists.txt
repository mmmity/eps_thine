cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)

project(util)

add_library(util
    numeric/numeric.cpp
)

target_include_directories(util
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/numeric
    ${CMAKE_CURRENT_SOURCE_DIR}/vis
)
