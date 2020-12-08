#include "MoveGenerator.hpp"

#include <iostream>

#include <NotationConversions.hpp>

namespace MoveGenerator
{

namespace
{

thread_local struct MoveContext{
	const Board* board;
	std::vector<Move>* allMoves;
	NOTATION::COLOR::color pieceColor;
} ctx;

bool isValid(const Move& move)
{
	Board boardCopy = *ctx.board;
	applyMove(boardCopy, move);
    return not isCheckOn(boardCopy, ctx.board->playerOnMove);
}

void generatePawnMoves(unsigned char i)
{
	if (ctx.pieceColor == NOTATION::COLOR::color::white)
	{
		if ((*ctx.board)[i + NOTATION::COORDINATES::ROW_DIFF] == 0)
		{
			Move m = Move(i, i + NOTATION::COORDINATES::ROW_DIFF);
			if (isValid(m))
			{
				ctx.allMoves->push_back(m);
			}
		}
		if ((*ctx.board)[i + 2 * NOTATION::COORDINATES::ROW_DIFF] == 0)
		{
			Move m = Move(i, i + 2* NOTATION::COORDINATES::ROW_DIFF);
			if (isValid(m))
			{
				ctx.allMoves->push_back(m);
			}
		}
	}
	else
	{

	}
}

void generateKnightMoves(unsigned char i)
{
	std::cout << (unsigned)i << std::endl;
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	static const std::pair<unsigned char, unsigned char> knightMoves[] = {
		{1, -2},
		{2, -1},
		{2, 1},
		{1, 2},
		{-1, 2},
		{-2, 1},
		{-2, -1},
		{-1, -2}
	};

	for (const auto& moveDiff : knightMoves)
	{
		unsigned char targerRow = row + moveDiff.first;
		unsigned char targerCol = col + moveDiff.second;
		if (NotationConversions::isColumnInBoard(targerCol)
			&& NotationConversions::isRowInBoard(targerRow))
		{
			unsigned char destination = NotationConversions::getFieldNum(targerRow, targerCol);
			const auto& field = (*ctx.board)[destination];
			if (field == 0
				or (static_cast<unsigned char>(NotationConversions::switchColor(ctx.pieceColor)) == (field & NOTATION::COLOR::COLOR_MASK)))
			{
				Move m{i, destination};
				if (isValid(m))
				{
					ctx.allMoves->push_back(m);
				}
			}
		}
	}
}

}  // namespace

std::vector<Move> MoveGenerator::generate(const Board& board,
	NOTATION::COLOR::color c) const
{
	std::vector<Move> allMoves;
	ctx.allMoves = &allMoves;
	ctx.board = &board;

	for (unsigned char i = 0u; i <= 64u; ++i)
	{
		if ((board[i] & NOTATION::COLOR_AND_PIECE_MASK)
			== (NOTATION::PIECES::PAWN | static_cast<unsigned char>(c)))
		{
			generatePawnMoves(i);
		}
		if ((board[i] & NOTATION::COLOR_AND_PIECE_MASK)
			== (NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(c)))
		{
			generateKnightMoves(i);
		}
	}
	return allMoves;
}

}  // namespace MoveGenerator
