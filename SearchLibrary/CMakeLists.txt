cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(SearchLibrary
        common/containers/HashTable.hpp
        common/containers/HashMap.hpp
        common/searchingAlgorithms/FullSearchingImplementation.hpp
        common/CachedEngines/FullCachedEngine.hpp
        common/CachedEngines/FullCachedEngine.cpp
        common/Constants.hpp)

#target_link_libraries(SearchLibrary BoardLibrary, StandardEvaluator)
