cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_target(applications)
add_executable(uci_application
        UciApplication/main.cpp
        utils/DebugWrapper.hpp UciApplication/EventsPropagator.hpp UciApplication/GameHandler.cpp UciApplication/GameHandler.hpp UciApplication/EventsPropagator.cpp UciApplication/Events.hpp utils/DebugWrapper.cpp)
target_link_libraries(uci_application board Players pthread)
add_dependencies(applications uci_application)
