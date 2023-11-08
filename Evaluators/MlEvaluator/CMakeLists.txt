cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(cppflow REQUIRED)

add_library(mlEvaluator SHARED
        ../evaluatorIf.hpp
        evaluator.cpp)

target_link_libraries(
        mlEvaluator
        cppflow::cppflow
)