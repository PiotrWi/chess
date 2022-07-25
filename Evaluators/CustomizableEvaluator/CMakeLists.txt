cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(CustomizableEvaluator SHARED
    ../evaluatorIf.hpp
    evaluator.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CustomizableEvaluatorConfig.xml ${PROJECT_SOURCE_DIR}/bin/CustomizableEvaluatorConfig.xml COPYONLY)
