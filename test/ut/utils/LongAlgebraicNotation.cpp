#include "LongAlgebraicNotation.hpp"

Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove)
{
	if (moveStr == "O-O")
	{
		if (playerOnMove == NOTATION::COLOR::color::white)
		{
			return Move{"e1", "g1"};
		}
		return Move{"e8", "g8"};
	}
	if (moveStr == "O-O-O")
	{
		if (playerOnMove == NOTATION::COLOR::color::white)
		{
			return Move{"e1", "c1"};
		}
        return Move{"e8", "c8"};
	}

	auto basePos = 0u;
	if (isupper(moveStr[0]))
	{
		basePos += 1;
	}

	return Move{moveStr.substr(basePos, 2).c_str()
		, moveStr.substr(basePos + 3, 2).c_str()};
}
