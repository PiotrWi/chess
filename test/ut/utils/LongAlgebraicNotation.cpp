#include "LongAlgebraicNotation.hpp"

#include <regex>

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

	std::smatch groups;
    std::regex moveTemplate(R"(.*([[:lower:]]\d)[-x]([[:lower:]]\d))");
    std::regex_match(moveStr, groups, moveTemplate);

	return Move{groups[1].str().c_str(), groups[2].str().c_str()};
}
