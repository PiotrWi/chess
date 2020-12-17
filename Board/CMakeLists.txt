cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(board
	Board.cpp
	BoardIO.cpp
	CheckChecker.cpp
	MoveGenerator.cpp
	MoveValidator.cpp
	NotationConversions.cpp
	ResultEvaluator.cpp)
	