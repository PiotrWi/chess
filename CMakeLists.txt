cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(chess LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS  "-O3 -Wall -Wextra -Werror -Wpedantic -pedantic-errors")

set(GTEST_INSTALATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/gtest/googltest )

include_directories(
	${PROJECT_SOURCE_DIR}/Board)

add_subdirectory(Board)
add_subdirectory(externals/gtest)
add_subdirectory(test/ut)

add_executable(chess main.cpp)

target_link_libraries(chess
	board)
