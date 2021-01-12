cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(board
                publicIf/Notation.hpp
                publicIf/Result.hpp
                publicIf/Board.hpp
                detail/Board.cpp
                publicIf/BoardIO.hpp
		detail/BoardIO.cpp
                detail/CheckChecker.hpp
		detail/CheckChecker.cpp
                detail/MoveGenerator.hpp
		detail/MoveGenerator.cpp
                detail/MoveValidator.hpp
		detail/MoveValidator.cpp
                publicIf/NotationConversions.hpp
		detail/NotationConversions.cpp
                detail/ResultEvaluator.hpp
		detail/ResultEvaluator.cpp
                detail/MoveApplier.hpp
		detail/MoveApplier.cpp
                BoardEngine.hpp
		BoardEngine.cpp
                notations/common.hpp
                notations/common.cpp
                notations/coordinationnotation.hpp
                notations/coordinationnotation.cpp
                notations/LongAlgebraicNotation.hpp
                notations/LongAlgebraicNotation.cpp
                hashing/zobrist.hpp
                hashing/Zobrish.cpp
                FieldLookup/FieldLookup.hpp)
	
