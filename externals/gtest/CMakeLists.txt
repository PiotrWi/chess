cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(ExternalProject)

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALATION_DIR}
)
