#include "MoveGenerator.hpp"

#include <publicIf/NotationConversions.hpp>
#include <detail/CheckChecker.hpp>
#include <detail/MoveApplier.hpp>

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
	MoveApplier::applyMove(boardCopy, move);
    return not CheckChecker::isCheckOn(boardCopy, ctx.board->playerOnMove);
}

void tryToAddMove(unsigned char source, unsigned char destination)
{
	Move m = Move(source, destination);
	if (isValid(m))
	{
		ctx.allMoves->push_back(m);
	}
}

template <unsigned char TARGET_LINE
	, unsigned char COLOR>
void tryToAddMoveAndPromote(unsigned char source, unsigned char destination)
{
	Move m = Move(source, destination);
	if (not isValid(m))
	{
		return;
	}
	if (NotationConversions::getRow(destination) == TARGET_LINE)
	{
		unsigned char pattern = COLOR | NOTATION::MOVED::MOVED_MASK;
		ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::QUEEN | pattern);
		ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::BISHOP | pattern);
		ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::ROCK | pattern);
		ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::KNIGHT | pattern);
		return;
	}
	ctx.allMoves->push_back(m);
}

template <unsigned char FIRST_LINE
	, unsigned char TARGET_LINE
	, signed char ROW_DIFF
	, unsigned char COLOR
	, unsigned char OPOSITE_COLOR>
void generateStandardPawnMoves(unsigned char i)
{
	if ((*ctx.board)[i + ROW_DIFF] == 0)
	{
		tryToAddMoveAndPromote<TARGET_LINE, COLOR>(i, i + ROW_DIFF);
		auto row = NotationConversions::getRow(i);
		if (row == FIRST_LINE and (*ctx.board)[i + 2 * ROW_DIFF] == 0)
		{
			tryToAddMove(i, i + 2 * ROW_DIFF);
		}
	}
	auto col = NotationConversions::getColumnNum(i);
	if (col<7)
	{
		auto destination = i + ROW_DIFF + 1;
		if ((*ctx.board)[destination] != 0 and
			((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
		{
			tryToAddMoveAndPromote<TARGET_LINE, COLOR>(i, destination);
		}
	}
	if (col > 0)
	{
		auto destination = i + ROW_DIFF - 1;
		if ((*ctx.board)[destination] != 0 and
			((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
		{
			tryToAddMoveAndPromote<TARGET_LINE, COLOR>(i, destination);
		}
	}
}

void generateEnPasant()
{
	if (ctx.pieceColor == NOTATION::COLOR::color::white)
	{
		auto& lastMove = ctx.board->lastMove;
		if (lastMove.source - lastMove.destination
			!= 2 * NOTATION::COORDINATES::ROW_DIFF
			or ((*ctx.board)[lastMove.destination] & NOTATION::PIECES::PIECES_MASK)
				!= NOTATION::PIECES::PAWN)
		{
			return;
		}
		auto col = NotationConversions::getColumnNum(lastMove.destination);
		if (col<7)
		{
			{
				auto opositeCandidateField = lastMove.destination + 1;
				if (((*ctx.board)[opositeCandidateField] & NOTATION::COLOR_AND_PIECE_MASK) ==
					(NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
				{
					tryToAddMove(opositeCandidateField,
						(lastMove.source + lastMove.destination) / 2);
				}
			}
			if (col > 0)
			{
				auto opositeCandidateField = lastMove.destination - 1;
				if (((*ctx.board)[opositeCandidateField] & NOTATION::COLOR_AND_PIECE_MASK) ==
					(NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
				{
					tryToAddMove(opositeCandidateField,
						(lastMove.source + lastMove.destination) / 2);
				}
			}
		}
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
				or (static_cast<unsigned char>(ctx.pieceColor+1) == (field & NOTATION::COLOR::COLOR_MASK)))
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

void generateCasles(unsigned char i)
{
	auto wasKingMoved = ((*ctx.board)[i] & NOTATION::MOVED::MOVED_MASK) != 0;
	if (wasKingMoved)
	{
		return;
	}

	if (CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i))
	{
		return;
	}

	const auto notMovedRockPattern = NOTATION::PIECES::ROCK;

	// long casle
	unsigned char quensRockPosition = i - 4;
	if (((*ctx.board)[quensRockPosition] & NOTATION::MOVED_AND_PIECE_MASK)
		== notMovedRockPattern
		and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i-1)
		and (*ctx.board)[i-1] == 0
		and (*ctx.board)[i-2] == 0
		and (*ctx.board)[i-3] == 0)
	{
		tryToAddMove(i, i-2);
	}

	// short castle
	unsigned char kingsRockPosition = i + 3;
	if (((*ctx.board)[kingsRockPosition] & NOTATION::MOVED_AND_PIECE_MASK)
		== notMovedRockPattern
		and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i+1)
		and (*ctx.board)[i+1] == 0
		and (*ctx.board)[i+2] == 0)
	{
		tryToAddMove(i, i+2);
	}
}

template <size_t N, const std::pair<unsigned char, unsigned char> TMoves[N]>
void generateLineMoves(unsigned char i)
{
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	for (auto* diff = TMoves; diff < TMoves + N; ++diff)
	{
		for (unsigned char r = row + diff->first, c = col + diff->second;
			r < 8u and c < 8u;
			r += diff->first, c+= diff->second)
		{
			auto destination = NotationConversions::getFieldNum(r, c);
			if ((*ctx.board)[destination] != 0)
			{
				if (static_cast<unsigned char>(ctx.pieceColor+1) ==
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
	NOTATION::COLOR::color c) const
{
	std::vector<Move> allMoves;
	ctx.allMoves = &allMoves;
	ctx.board = &board;
	ctx.pieceColor = c;

	for (unsigned char i = 0u; i <= 64u; ++i)
	{
		switch ((board[i] & NOTATION::COLOR_AND_PIECE_MASK) ^ static_cast<unsigned char>(c))
		{
		case (NOTATION::PIECES::PAWN):
			if (c == NOTATION::COLOR::color::white)
			{
				generateStandardPawnMoves<1u, 7u,
					NOTATION::COORDINATES::ROW_DIFF,
					NOTATION::COLOR::WHITE,
					NOTATION::COLOR::BLACK>(i);
			}
			else
			{
				generateStandardPawnMoves<6u, 0u,
					-NOTATION::COORDINATES::ROW_DIFF,
					NOTATION::COLOR::BLACK,
					NOTATION::COLOR::WHITE>(i);
			}
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
			generateCasles(i);
			continue;
		}
	}
	generateEnPasant();
	return allMoves;
}

}  // namespace MoveGenerator
