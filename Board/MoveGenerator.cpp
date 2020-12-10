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

template <size_t N, const std::pair<unsigned char, unsigned char> TMoves[N]>
void generateFixedMoves(unsigned char i)
{
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	for (auto* diff = TMoves; diff < TMoves + N; ++diff)
	{
		unsigned char targerRow = row + diff->first;
		unsigned char targerCol = col + diff->second;
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

const std::pair<unsigned char, unsigned char> knightMoves[] = {
	{1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };
void (*generateKnightMoves)(unsigned char i) = generateFixedMoves<8, knightMoves>;

const std::pair<unsigned char, unsigned char> kingMoves[] = {
	{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
void (*generateNormalKingMoves)(unsigned char i) = generateFixedMoves<8, kingMoves>;

template <size_t N, const std::pair<unsigned char, unsigned char> TMoves[N]>
void generateLineMoves(unsigned char i)
{
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	for (auto* diff = TMoves; diff < TMoves + N; ++diff)
	{
		std::cout << "in " << (unsigned) row << " " << (unsigned) col <<std::endl;
		for (unsigned char r = row + diff->first, c = col + diff->second;
			r < 8u and c < 8u;
			r += diff->first, c+= diff->second)
		{
			std::cout << "check " << (unsigned) r << " " << (unsigned) c <<std::endl;
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

const std::pair<unsigned char, unsigned char> rockMoves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
void (*generateRockMoves)(unsigned char i) = generateLineMoves<4, rockMoves>;

const std::pair<unsigned char, unsigned char> bishopMoves[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
void (*generateBishopMoves)(unsigned char i) = generateLineMoves<4, bishopMoves>;

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
		// TODO the same for black
		case (NOTATION::PIECES::PAWN):
			generatePawnMoves(i);
			// TODO: en passant
			// TODO: promotions
			continue;
		case (NOTATION::PIECES::KNIGHT):
			generateKnightMoves(i);
			continue;
		case (NOTATION::PIECES::ROCK):
			generateRockMoves(i);
			continue;
		case (NOTATION::PIECES::BISHOP):
			generateBishopMoves(i);
			continue;
		case (NOTATION::PIECES::QUEEN):
			generateBishopMoves(i);
			generateRockMoves(i);
			continue;
		case (NOTATION::PIECES::KING):
			generateNormalKingMoves(i);
			// TODO: Castling
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
