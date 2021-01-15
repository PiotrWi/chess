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
        common/MoveGenerators/CachedMoveGenerator.hpp
        common/MoveGenerators/CachedMoveGenerator.cpp
        common/containers/HashMap.hpp
        common/MoveGenerators/CachedMoveGeneratorMap.cpp
        common/MoveGenerators/CachedMoveGeneratorMap.hpp
        common/MoveGenerators/MoveGenerator.hpp common/MoveGenerators/MoveGenerator.cpp ComputerPlayer/CachedPlayer.cpp ComputerPlayer/CachedPlayer.hpp common/searchingAlgorithms/AlfaBetaPvs.hpp)

target_link_libraries(Players board)