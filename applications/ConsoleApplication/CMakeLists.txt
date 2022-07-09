cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR})
    
add_executable(ConsolleApplication
    main.cpp
    IPLayer.hpp
    ConsoleApplication.cpp
    ConsoleApplication.hpp
    ComputerPlayer/CachedPlayer.cpp
    ComputerPlayer/CachedPlayer.hpp
    HumanPlayer/HumanPlayer.hpp
    HumanPlayer/HumanPlayer.cpp
)

target_link_libraries(ConsolleApplication
    BoardLibrary
    SearchLibrary
    StandardEvaluator)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
