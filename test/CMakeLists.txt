include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_subdirectory(ut)
add_subdirectory(performanceTests)
