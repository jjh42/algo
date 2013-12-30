cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
project(algo)
enable_testing()

add_library(median median.c)

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
set(LIBS ${LIBS} ${CHECK_LIBRARIES} median)
add_executable(check_median check_median.c)
target_link_libraries(check_median ${LIBS})
add_test(check_median ${CMAKE_CURRENT_BINARY_DIR}/check_median)
