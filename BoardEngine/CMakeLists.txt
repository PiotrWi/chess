cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(board
                publicIf/Board.hpp
                detail/Board.cpp
		detail/BoardIO.cpp
		detail/CheckChecker.cpp
		detail/MoveGenerator.cpp
		detail/MoveValidator.cpp
                publicIf/NotationConversions.hpp
		detail/NotationConversions.cpp
		detail/ResultEvaluator.cpp
                detail/MoveApplier.hpp
		detail/MoveApplier.cpp
		BoardEngine.cpp
                notations/LongAlgebraicNotation.hpp
                notations/LongAlgebraicNotation.cpp
                hashing/zobrist.hpp
                hashing/Zobrish.cpp
                FieldLookup/FieldLookup.hpp)
	
