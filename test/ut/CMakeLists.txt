cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

get_target_property(UCI_APP_INCLUDES UciApplication-lib INCLUDE_DIRECTORIES)

include_directories(${GTEST_INSTALATION_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${UCI_APP_INCLUDES}
    )
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
    MaterialEvaluatorTest.cpp
    SearchingBasicTests.cpp
    MoveApplierTest.cpp
    MoveGeneratorTest.cpp
    MoveGeneratorV2Test.cpp
    MoveValidatorTest.cpp
    pinnedchecktest.cpp
    ResultEvaluatorTest.cpp
    ZobrishTest.cpp
    mt/WholeGame1.cpp
    mt/WholeGame2.cpp
    mt/WholeGame3.cpp
    CommandParserTest.cpp
    uciNotationTest.cpp
    seeTest.cpp
    PreorderedMovesTests.cpp
    PoolAllocatorTests.cpp
)

add_dependencies(ut googletest)
target_link_libraries(ut gtest gmock gtest_main pthread BoardLibrary SearchLibrary StandardEvaluator dl UciApplication-lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_test(
        NAME ut-run
        COMMAND $<TARGET_FILE:ut>
)
