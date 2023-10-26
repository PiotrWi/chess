cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(benchmark REQUIRED)
include_directories(${benchmark_INCLUDE_DIRS})

find_package(GTest REQUIRED)
include_directories(${gtest_INCLUDE_DIRS})

include_directories(${GTEST_INSTALATION_DIR}/include ${BENCHMARK_INSTALATION_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)
link_directories(${BENCHMARK_INSTALATION_DIR}/lib)

add_executable(pt
        main.cpp
        ../utils/BoardGenerationUtils.cpp
        FullSearchTest.cpp
        MoveGeneratorV2Tests.cpp
        PreorderedMovesTests.cpp
        StabilityBasedTests.cpp
        )

target_link_libraries(pt
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        GTest::gmock_main
        benchmark::benchmark
        pthread
        BoardLibrary
        SearchLibrary
        StandardEvaluator
        dl
        )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_test(
        NAME pt-run
        COMMAND $<TARGET_FILE:pt>
)
