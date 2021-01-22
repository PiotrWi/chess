cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_custom_target(pt-run)
include_directories(${GTEST_INSTALATION_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_INSTALATION_DIR}/lib)

add_executable(pt
        main.cpp
        ../utils/BoardGenerationUtils.cpp
        MinMaxTests.cpp
        AlfaBetaTests.cpp
        AlfaBetaWithTransictionsTest.cpp
        AldaBetaWithTransictionMapTest.cpp
        #FullSimulation.cpp
        AlfaBetaPvs.cpp AlfaBetaMtdPvs.cpp AlfaBetaHistory.cpp)


add_dependencies(pt googletest)
add_dependencies(pt board)

target_link_libraries(pt gtest gmock gtest_main pthread board Players)
add_dependencies(pt-run pt)
