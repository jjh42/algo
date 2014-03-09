cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
project(algo)
enable_testing()

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
set(LIBS ${LIBS} ${CHECK_LIBRARIES} median)


add_library(median median.c)
add_executable(check_median check_median.c)
target_link_libraries(check_median ${LIBS})
add_test(check_median ${CMAKE_CURRENT_BINARY_DIR}/check_median)


add_library(map btree.c hash.c red_black.c map.c)
add_executable(check_map check_map.c check_red_black.c check_hash.c check_btree.c)
target_link_libraries(check_map map ${LIBS})
add_test(check_map ${CMAKE_CURRENT_BINARY_DIR}/check_map)

add_executable(benchmark_map benchmark_map.c)
target_link_libraries(benchmark_map map ${LIBS})
