cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(Players
        common/IPLayer.hpp
        common/evaluators/MaterialEvaluator.hpp
        common/evaluators/MaterialEvaluator.cpp
        common/evaluators/MoveCountEvaluator.hpp
        common/evaluators/MoveCountEvaluator.cpp
        common/searchingAlgorithms/MinMax.hpp
        common/searchingAlgorithms/MinMax.cpp
        ComputerPlayer/SimpleComputerPlayer.cpp
        ComputerPlayer/SimpleComputerPlayer.hpp
        common/searchingAlgorithms/AlfaBeta.hpp
        common/searchingAlgorithms/AlfaBeta.cpp
        common/containers/HashTable.hpp
        common/CachedMoveGenerator/CachedMoveGenerator.hpp
        common/CachedMoveGenerator/CachedMoveGenerator.cpp
        common/searchingAlgorithms/AlfaBetaWithTransitctionTable.hpp
        common/searchingAlgorithms/AlfaBetaWithTransitcionTable.cpp
        common/containers/HashMap.hpp
        common/CachedMoveGenerator/CachedMoveGeneratorMap.cpp
        common/CachedMoveGenerator/CachedMoveGeneratorMap.hpp
        common/searchingAlgorithms/AlfaBetaTransictionTableMap.cpp
        common/searchingAlgorithms/AlfaBetaTransictionTableMap.hpp
        HumanPlayer/HumanPlayer.hpp
        HumanPlayer/HumanPlayer.cpp)

target_link_libraries(Players board)
