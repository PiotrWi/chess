cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(SearchLibrary
        searchingAlgorithms/FullSearchingImplementation.hpp
        CachedEngines/FullCachedEngine.hpp
        CachedEngines/FullCachedEngine.cpp
        Constants.hpp searchingAlgorithms/MVVLVA.hpp
        searchingAlgorithms/OnlyBeatingMoves.hpp)
