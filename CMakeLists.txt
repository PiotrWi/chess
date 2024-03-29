cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# section flags
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message (STATUS "CCache avaiable")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

project(chess LANGUAGES CXX)
#add_definitions(-DUSE_AVX)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DASSERTSON)
    add_definitions(-DUSE_PEXT)
    SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -std=c++20 -mbmi2 -mavx -march=haswell -fuse-ld=gold -lrt -latomic -pthread")
else()
    add_definitions(-DUSE_PEXT)
    SET(CMAKE_CXX_FLAGS  "-Ofast -Wall -Wextra -Werror -std=c++20 -mbmi2 -mavx -march=haswell -msse4.1 -flto -fuse-ld=gold -m64 -lrt -latomic -pthread")
endif()

# end of section flags

set(GTEST_INSTALATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/gtest/googltest )
set(BENCHMARK_INSTALATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/benchmark/benchmark)
set(NON_STD_INSTALATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/non_std)

include_directories(
    ${PROJECT_SOURCE_DIR}/BoardLibrary/
    ${PROJECT_SOURCE_DIR}/SearchLibrary
    ${PROJECT_SOURCE_DIR}/Evaluators
    ${NON_STD_INSTALATION_DIR}/non_std)

link_directories(
    ${PROJECT_SOURCE_DIR}/BoardLibrary/
    ${PROJECT_SOURCE_DIR}/SearchLibrary
    ${PROJECT_SOURCE_DIR}/Evaluators)

add_subdirectory(BoardLibrary)
add_subdirectory(SearchLibrary)
add_subdirectory(Evaluators)
add_subdirectory(applications)

enable_testing()
add_subdirectory(externals/non_std)

add_subdirectory(test)

