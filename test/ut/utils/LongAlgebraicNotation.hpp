#pragma once

#include <string>
#include <publicIf/Board.hpp>

// Long algebraic notation
Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove);
