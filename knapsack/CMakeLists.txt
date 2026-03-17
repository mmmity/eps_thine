cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)

project(knapsack)

add_library(solver STATIC
	solver/greedy_solver.cpp
	solver/solver.cpp
)

target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(knapsack main.cpp)
target_link_libraries(knapsack PRIVATE solver)

