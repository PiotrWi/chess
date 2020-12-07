#pragma once

#include <string>
#include <Board.hpp>

// Long algebraic notation
Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove);
