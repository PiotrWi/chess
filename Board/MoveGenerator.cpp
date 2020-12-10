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

void tryToAddMove(unsigned char source, unsigned char destination)
{
	Move m = Move(source, destination);
	if (isValid(m))
	{
		ctx.allMoves->push_back(m);
	}
}

void generatePawnMoves(unsigned char i)
{
	if (ctx.pieceColor == NOTATION::COLOR::color::white)
	{
		if ((*ctx.board)[i + NOTATION::COORDINATES::ROW_DIFF] == 0)
		{
			tryToAddMove(i, i + NOTATION::COORDINATES::ROW_DIFF);
			auto row = NotationConversions::getRow(i);
			if (row == 1 and (*ctx.board)[i + 2 * NOTATION::COORDINATES::ROW_DIFF] == 0)
			{
				tryToAddMove(i, i + 2 * NOTATION::COORDINATES::ROW_DIFF);
			}
		}
		auto col = NotationConversions::getColumnNum(i);
		if (col<7)
		{
			auto destination = i + NOTATION::COORDINATES::ROW_DIFF + 1;
			if ((*ctx.board)[destination] != 0 and
				((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == NOTATION::COLOR::BLACK)
			{
				tryToAddMove(i, destination);
			}
		}
		if (col > 0)
		{
			auto destination = i + NOTATION::COORDINATES::ROW_DIFF - 1;
			if ((*ctx.board)[destination] != 0 and
				((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == NOTATION::COLOR::BLACK)
			{
				tryToAddMove(i, destination);
			}
		}
	}
	else
	{

	}
}

void generateKnightMoves(unsigned char i)
{
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

void generateRockMoves(unsigned char i)
{
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	static const std::pair<unsigned char, unsigned char> rockMoves[] = {
		{-1, 0},
		{1, 0},
		{0, -1},
		{0, 1},
	};

	for (const auto& diff: rockMoves)
	{
		for (unsigned char r = row, c = col;
			r < 8 and c < 8;
			r += diff.first, c+= diff.second)
		{
			auto destination = NotationConversions::getFieldNum(r, c);
			if ((*ctx.board)[destination] != 0)
			{
				if (static_cast<unsigned char>(NotationConversions::switchColor(ctx.pieceColor)) ==
					((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK))
				{
					tryToAddMove(i, destination);
				}
				break;
			}
			tryToAddMove(i, destination);
		}
	}
}

}  // namespace

std::vector<Move> MoveGenerator::generate(const Board& board,
	NOTATION::COLOR::color) const
{
	std::vector<Move> allMoves;
	ctx.allMoves = &allMoves;
	ctx.board = &board;

	for (unsigned char i = 0u; i <= 64u; ++i)
	{
		switch (board[i] & NOTATION::COLOR_AND_PIECE_MASK)
		{
		case (NOTATION::PIECES::PAWN):
			generatePawnMoves(i);
			continue;
		case (NOTATION::PIECES::KNIGHT):
			generateKnightMoves(i);
			continue;
		case (NOTATION::PIECES::ROCK):
			generateRockMoves(i);
			continue;

		}
	}
	for (auto&&m : allMoves)
	{
		std::cout << m << std::endl;
	}
	return allMoves;
}

}  // namespace MoveGenerator
