cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_custom_target(ut-run)
include_directories(${GTEST_INSTALATION_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)

add_executable(ut
	main.cpp
	CheckCheckerTest.cpp
	MoveValidatorTest.cpp
	MoveGeneratorTest.cpp
	ResultEvaluatorTest.cpp
	../utils/BoardGenerationUtils.cpp
	MoveApplierTest.cpp
	MaterialEvaluatorTest.cpp
	MinMaxAndAlfaBetaTest.cpp ZobrishTest.cpp HashTableTest.cpp CachedMoveGeneratorTest.cpp HashMapTest.cpp BoardTest.cpp FullSearchTest.cpp)


add_dependencies(ut googletest)
add_dependencies(ut board)

target_link_libraries(ut gtest gmock gtest_main pthread board Players)
add_dependencies(ut-run ut)
