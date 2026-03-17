cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)

project(eps_thine)

add_subdirectory(util)


add_library(setcover_solver STATIC
	setcover/solver/greedy_solver.cpp
	setcover/solver/solver.cpp
)
target_include_directories(setcover_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/setcover/solver)
target_link_libraries(setcover_solver PRIVATE util)

add_library(knapsack_solver STATIC
	knapsack/solver/greedy_solver.cpp
	knapsack/solver/solver.cpp
)
target_include_directories(knapsack_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/knapsack/solver)
target_link_libraries(knapsack_solver PRIVATE util)

add_library(coloring_solver STATIC
	coloring/solver/greedy_solver.cpp
	coloring/solver/solver.cpp
)
target_include_directories(coloring_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/coloring/solver)
target_link_libraries(coloring_solver PRIVATE util)

add_executable(coloring coloring/main.cpp)
add_executable(coloring_checker coloring/checker.cpp)
target_link_libraries(coloring PRIVATE coloring_solver)

add_executable(setcover setcover/main.cpp)
add_executable(setcover_checker setcover/checker.cpp)
target_link_libraries(setcover PRIVATE setcover_solver)

add_executable(knapsack knapsack/main.cpp)
add_executable(knapsack_checker knapsack/checker.cpp)
target_link_libraries(knapsack PRIVATE knapsack_solver)
