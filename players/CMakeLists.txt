cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(Players
        ConsoleApplication.hpp
        ConsoleApplication.cpp
        HumanPlayer/HumanPlayer.cpp
        common/IPLayer.hpp
        common/evaluators/MaterialEvaluator.hpp
        common/evaluators/MaterialEvaluator.cpp
        common/evaluators/MoveCountEvaluator.cpp
        common/containers/HashTable.hpp
        common/containers/HashMap.hpp
        ComputerPlayer/CachedPlayer.cpp
        ComputerPlayer/CachedPlayer.hpp
        common/evaluators/PositionEvaluator.hpp
        common/evaluators/PawnStructureEvaluator.hpp
        common/evaluators/PawnStructureEvaluator.cpp
        common/searchingAlgorithms/FullSearchingImplementation.hpp
        common/CachedEngines/FullCachedEngine.hpp
        common/CachedEngines/FullCachedEngine.cpp
        common/Constants.hpp common/evaluators/PositionEvaluator.cpp)

target_link_libraries(Players board)