cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(board
		detail/Board.cpp
		detail/BoardIO.cpp
		detail/CheckChecker.cpp
		detail/MoveGenerator.cpp
		detail/MoveValidator.cpp
		detail/NotationConversions.cpp
		detail/ResultEvaluator.cpp detail/MoveApplier.cpp BoardEngine.hpp publicIf/Result.hpp BoardEngine.cpp)
	