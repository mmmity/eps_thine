cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)

set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${SANITIZER_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

project(eps_thine)

add_subdirectory(util)


add_library(setcover_solver STATIC
	setcover/solver/greedy_solver.cpp
	setcover/solver/bnb_solver.cpp
	setcover/solver/randomized_solver.cpp
	setcover/solver/randomized_element_solver.cpp
	setcover/solver/solver.cpp
)
target_include_directories(setcover_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/setcover/solver)
target_link_libraries(setcover_solver PUBLIC util)

add_library(knapsack_solver STATIC
	knapsack/solver/bnb_solver.cpp
	knapsack/solver/greedy_solver.cpp
	knapsack/solver/solver.cpp
)
target_include_directories(knapsack_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/knapsack/solver)
target_link_libraries(knapsack_solver PUBLIC util)
if (MSVC)
    target_link_options(knapsack_solver PRIVATE /STACK:67108864)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(knapsack_solver PRIVATE -Wl,-z,stack-size=67108864)
elseif (APPLE)
    target_link_options(knapsack_solver PRIVATE -Wl,-stack_size,0x4000000)
endif()

add_library(coloring_solver STATIC
	coloring/solver/greedy_solver.cpp
	coloring/solver/ld_solver.cpp
	coloring/solver/solver.cpp
)
target_include_directories(coloring_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/coloring/solver)
target_link_libraries(coloring_solver PUBLIC util)

add_library(tsp_solver STATIC
	tsp/solver/anneal_solver.cpp
	tsp/solver/flood_solver.cpp
	tsp/solver/ls_solver.cpp
	tsp/solver/greedy_solver.cpp
	tsp/solver/solver.cpp
)
target_include_directories(tsp_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tsp/solver)
target_link_libraries(tsp_solver PUBLIC util)

add_library(facility_solver STATIC
	facility/solver/anneal_solver.cpp
	facility/solver/bnb_solver.cpp
	facility/solver/greedy_solver.cpp
	facility/solver/solver.cpp
)
target_include_directories(facility_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/facility/solver ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(facility_solver PUBLIC util knapsack_solver)

add_library(vrp_solver STATIC
	vrp/solver/kmeans_solver.cpp
	vrp/solver/greedy_solver.cpp
	vrp/solver/solver.cpp
)
target_include_directories(vrp_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vrp/solver ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vrp_solver PUBLIC util tsp_solver)

add_executable(coloring coloring/main.cpp)
add_executable(coloring_checker coloring/checker.cpp)
target_link_libraries(coloring PRIVATE coloring_solver)

add_executable(setcover setcover/main.cpp)
add_executable(setcover_checker setcover/checker.cpp)
target_link_libraries(setcover PRIVATE setcover_solver)

add_executable(knapsack knapsack/main.cpp)
add_executable(knapsack_checker knapsack/checker.cpp)
target_link_libraries(knapsack PRIVATE knapsack_solver)

add_executable(tsp tsp/main.cpp)
add_executable(tsp_checker tsp/checker.cpp)
target_link_libraries(tsp PRIVATE tsp_solver)

add_executable(facility facility/main.cpp)
add_executable(facility_checker facility/checker.cpp)
target_link_libraries(facility PRIVATE facility_solver)

add_executable(vrp vrp/main.cpp)
add_executable(vrp_checker vrp/checker.cpp)
target_link_libraries(vrp PRIVATE vrp_solver)