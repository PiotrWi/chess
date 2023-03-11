#include "BoardGenerationUtils.hpp"
#include "core/NotationConversions.hpp"

#include <cstring>
#include <string>
#include <utility>

namespace
{

std::pair<const char*, unsigned char> symbolToPieceMapping[] = {
	std::make_pair("♜", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♞", NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT),
	std::make_pair("♝", NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP),
	std::make_pair("♛", NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN),
	std::make_pair("♚", NOTATION::COLOR::BLACK | NOTATION::PIECES::KING),
	std::make_pair("♟", NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN),
	std::make_pair("♙", NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN),
	std::make_pair("♕", NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN),
	std::make_pair("♔", NOTATION::COLOR::WHITE | NOTATION::PIECES::KING),
	std::make_pair("♗", NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP),
	std::make_pair("♘", NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT),
	std::make_pair("♖", NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK),
};

int unicodeLen(unsigned char c)
{
	if (~c & 0b1000000)
		return 1;
	if ((c & 0b11110000) == 0b11110000)
		return 4;
	if ((c & 0b11100000) == 0b11100000)
		return 3;
	if ((c & 0b11000000) == 0b11000000)
		return 2;
	return 1;
}

unsigned char getCorrespondingFigure(const char* str, size_t bytesLen)
{
	for (const auto& map : symbolToPieceMapping)
	{
		bool match = true;
        for (size_t i = 0; i< bytesLen; ++i)
		{
			match &= map.first[i] == str[i];
		}
		if (match)
		{
			return map.second;
		}
	}
	return 0;
}

}

namespace utils
{

const char* InitialBoardString =
		    "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "        "
            "        "
            "♙♙♙♙♙♙♙♙"
            "♖♘♗♕♔♗♘♖";

Board createBoard(const char* position,
        NOTATION::COLOR::color playerOnMove)
{
	Board board;
    for (size_t str_i = 0, i = 0; str_i < strlen(position); ++i)
	{
		auto len = unicodeLen(position[str_i]);
		if (len != 1)
		{
			auto piece = getCorrespondingFigure(&position[str_i], len);
			auto row = 7 - (i / 8);
			auto col = i & 0b111;

            board.setField((row << 3) | col, piece);
		}
		str_i += len;
	}
    board.validEnPassant = -1;
    board.playerOnMove = playerOnMove;
    board.castlingRights = NOTATION::CASTLING_RIGHTS::CASTLING_MASK;

	return board;
}

void revokeCastlingRights(Board& board, unsigned char rightBit)
{
    board.castlingRights &= (~rightBit);
}

void setValidEnPassant(Board& board, const char* posStr)
{
    board.validEnPassant = NotationConversions::getFieldNum(posStr);
}

uint64_t convertToUint64(const std::string& s)
{
    uint64_t out = {};
    for (unsigned int i = 0; i < 64; ++i)
    {
        if (s[i] == 'x')
        {
            auto row = 7 - (i / 8);
            auto col = i & 0b111;
            out |= 1ull << ((row << 3) | col);
        }
    }
    return out;
}

}  // namespace utils
