cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ConsoleApplication)

add_custom_target(applications)

add_library(UciApplication-lib
    utils/DebugWrapper.hpp
    UciApplication/EventsPropagator.hpp
    UciApplication/GameHandler.cpp
    UciApplication/GameHandler.hpp
    UciApplication/EventsPropagator.cpp
    UciApplication/Events.hpp
    utils/DebugWrapper.cpp
    utils/Timer.hpp
    utils/Timer.cpp)

add_executable(UciApplication
    UciApplication/main.cpp)

target_link_libraries(UciApplication
    pthread
    UciApplication-lib
    BoardLibrary
    SearchLibrary
    StandardEvaluator)

add_executable(MagicBbGenerator
    MagicBbGenerator/main.cpp)
target_link_libraries(MagicBbGenerator BoardLibrary)

add_dependencies(applications UciApplication)
add_dependencies(applications MagicBbGenerator)
add_dependencies(applications ConsolleApplication)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
