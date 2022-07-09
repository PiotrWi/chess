cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_custom_target(ut-run)
include_directories(${GTEST_INSTALATION_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)

add_executable(ut
	main.cpp
        ../utils/BoardGenerationUtils.cpp
        BoardTest.cpp
        CheckCheckerTest.cpp
        fentest.cpp
        FullSearchTest.cpp
        HashMapTest.cpp
        HashTableTest.cpp
        #MaterialEvaluatorTest.cpp
        SearchingBasicTests.cpp
        MoveApplierTest.cpp
        MoveGeneratorTest.cpp
	MoveValidatorTest.cpp
        pinnedchecktest.cpp
        ResultEvaluatorTest.cpp
        ZobrishTest.cpp
        mt/WholeGame1.cpp
        mt/WholeGame2.cpp
        mt/WholeGame3.cpp)

add_dependencies(ut googletest)

target_link_libraries(ut gtest gmock gtest_main pthread BoardLibrary SearchLibrary StandardEvaluator)
add_dependencies(ut-run ut)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
