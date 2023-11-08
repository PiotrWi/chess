cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_custom_target(evaluators)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

include_directories(
    ${PROJECT_SOURCE_DIR}/Common)

add_subdirectory(StandardEvaluator)
add_dependencies(evaluators StandardEvaluator)

add_subdirectory(CustomizableEvaluator)
add_dependencies(evaluators CustomizableEvaluator)

add_subdirectory(MlEvaluator)
add_dependencies(evaluators mlEvaluator)
