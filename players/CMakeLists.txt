cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(Players
        ConsoleApplication.hpp
        ConsoleApplication.cpp
        HumanPlayer/HumanPlayer.cpp
        common/IPLayer.hpp
        common/evaluators/MaterialEvaluator.hpp
        common/evaluators/MaterialEvaluator.cpp
        common/searchingAlgorithms/MinMax.hpp
        common/searchingAlgorithms/MinMax.cpp
        ComputerPlayer/SimpleComputerPlayer.cpp
        ComputerPlayer/SimpleComputerPlayer.hpp
        common/searchingAlgorithms/AlfaBeta.hpp
        common/evaluators/MoveCountEvaluator.cpp
        common/containers/HashTable.hpp
        common/CachedEngines/CachedMoveGenerator.hpp
        common/CachedEngines/CachedMoveGenerator.cpp
        common/containers/HashMap.hpp
        common/CachedEngines/CachedEngineWithMap.cpp
        common/CachedEngines/CachedEngineWithMap.hpp
        common/CachedEngines/MoveGenerator.hpp common/CachedEngines/MoveGenerator.cpp ComputerPlayer/CachedPlayer.cpp ComputerPlayer/CachedPlayer.hpp common/searchingAlgorithms/AlfaBetaPvs.hpp common/evaluators/MaterialAndMoveCountEvaluator.hpp common/evaluators/PawnStructureEvaluator.hpp common/evaluators/PawnStructureEvaluator.cpp)

target_link_libraries(Players board)