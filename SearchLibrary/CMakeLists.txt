cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(SearchLibrary
        common/searchingAlgorithms/FullSearchingImplementation.hpp
        common/CachedEngines/FullCachedEngine.hpp
        common/CachedEngines/FullCachedEngine.cpp
        common/Constants.hpp common/searchingAlgorithms/MVVLVA.hpp
        common/searchingAlgorithms/OnlyBeatingMoves.hpp)
