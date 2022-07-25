cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include_directories(${GTEST_INSTALATION_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)

add_executable(pt
        main.cpp
        ../utils/BoardGenerationUtils.cpp
        FullSearchTest.cpp)

add_dependencies(pt googletest)
target_link_libraries(pt gtest gmock gtest_main pthread BoardLibrary SearchLibrary StandardEvaluator dl)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_test(
        NAME pt-run
        COMMAND $<TARGET_FILE:pt>
)
