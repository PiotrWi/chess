cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(Players
        HumanPlayer/HumanPlayer.cpp common/IPLayer.hpp common/evaluators/MaterialEvaluator.hpp common/evaluators/MaterialEvaluator.cpp common/searchingAlgorithms/MinMax.hpp common/searchingAlgorithms/MinMax.cpp)
