#pragma once

#include <string>
#include <vector>
#include <publicIf/Board.hpp>

// Long algebraic notation
Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove);
std::vector<char> createMoveStr(const Move&);
