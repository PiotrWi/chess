cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(board
	Board.cpp
	CheckChecker.cpp
	MoveValidator.cpp
	NotationConversions.cpp
	Pieces.cpp)
	