cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_custom_target(ut-run)
include_directories(${GTEST_INSTALATION_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)

add_executable(ut
	main.cpp
        ../utils/BoardGenerationUtils.cpp
        BoardTest.cpp
        CachedMoveGeneratorTest.cpp
	CheckCheckerTest.cpp
        FullSearchTest.cpp
        HashMapTest.cpp
        HashTableTest.cpp
        MaterialEvaluatorTest.cpp
        MinMaxAndAlfaBetaTest.cpp
        MoveApplierTest.cpp
        MoveGeneratorTest.cpp
	MoveValidatorTest.cpp
        pinnedchecktest.cpp
        ResultEvaluatorTest.cpp
        ZobrishTest.cpp)

add_dependencies(ut googletest)
add_dependencies(ut board)

target_link_libraries(ut gtest gmock gtest_main pthread board Players)
add_dependencies(ut-run ut)
