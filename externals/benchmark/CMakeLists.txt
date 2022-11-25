cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(ExternalProject)

ExternalProject_Add(googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${BENCHMARK_INSTALATION_DIR} -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
    GIT_TAG d572f4777349d43653b21d6c2fc63020ab326db2
)
