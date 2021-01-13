cmake_minimum_required(VERSION 3.5 FATAL_ERROR)


# section flags
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
	message (STATUS "CCache avaiable")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(chess LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS  "-O3 -Wall -Wextra -Werror -Wpedantic -pedantic-errors -std=c++17 -flto -fuse-ld=gold -m64")
#SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -Wpedantic -pedantic-errors -std=c++17 -fuse-ld=gold")

# end of section flags

set(GTEST_INSTALATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/gtest/googltest )
include_directories(
		${PROJECT_SOURCE_DIR}/BoardEngine
		${PROJECT_SOURCE_DIR}/players)

add_subdirectory(BoardEngine)
add_subdirectory(players)
add_subdirectory(externals/gtest)
add_subdirectory(test)

add_executable(chess main.cpp)

target_link_libraries(chess
	board
	Players)
