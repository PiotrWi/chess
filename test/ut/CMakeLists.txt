cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include_directories(${GTEST_INSTALATION_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)

add_executable(ut main.cpp CheckCheckerTest.cpp MoveValidatorTest.cpp)
add_dependencies(ut googletest)
add_dependencies(ut board)

target_link_libraries(ut gtest gtest_main pthread board)
