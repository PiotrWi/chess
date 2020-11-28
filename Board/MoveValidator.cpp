#include "MoveValidator.hpp"

#include <utility>

#include <Board.hpp>
#include <NotationConversions.hpp>

namespace
{

thread_local struct MoveContext{
	const Board* board;
	const Move* move;
	NOTATION::COLOR::color pieceColor;
	unsigned char piece;
	unsigned char sourceRow;
	unsigned char sourceColumn;
	unsigned char targetRow;
	unsigned char targetColumn;
} ctx;

bool valiateConcretePiece();
bool validatePawn();
bool validateRock();
bool validateBishop();
bool validateKnight();
bool validateKing();

bool valiateConcretePiece()
{
	auto pieceType = ctx.piece & NOTATION::PIECES::PIECES_MASK;

	if (pieceType == NOTATION::PIECES::PAWN)
	{
		return validatePawn();
	}
	if (pieceType == NOTATION::PIECES::ROCK)
	{
		return validateRock();
	}
	if (pieceType == NOTATION::PIECES::BISHOP)
	{
		return validateBishop();
	}
	if (pieceType == NOTATION::PIECES::KNIGHT)
	{
		return validateKnight();
	}
	if (pieceType == NOTATION::PIECES::QUEEN)
	{
		return validateRock() or validateBishop();
	}
	if (pieceType == NOTATION::PIECES::KING)
	{
		return validateKing();
	}
	return false;
}

bool noCheckAfterMove()
{
	Board boardCopy = *ctx.board;
	applyMove(boardCopy, *ctx.move);
	return isCheckOn(boardCopy, ctx.board->playerOnMove);
}

bool validatePawn()
{
	constexpr unsigned char singeRow = 8u;
	auto isBeating = ctx.sourceColumn == ctx.targetColumn;
	auto rowDifference = ctx.targetRow - ctx.sourceRow;

	if (not isBeating)
	{
		if (ctx.pieceColor == NOTATION::COLOR::color::white)
		{
			if (rowDifference == 2)
			{
				return ctx.sourceRow == 1
					and ((*ctx.board)[ctx.move->source + singeRow] == 0u)
					and ((*ctx.board)[ctx.move->source + 2*singeRow] == 0u);
			}
			if (rowDifference == 1
				and ctx.pieceColor == NOTATION::COLOR::color::white)
			{
				return ((*ctx.board)[ctx.move->source + singeRow] == 0u);
			}
		}
		else
		{
			if (rowDifference == -2)
			{
				return ctx.sourceRow == 6
					and ((*ctx.board)[ctx.move->source - singeRow] == 0u)
					and ((*ctx.board)[ctx.move->source - 2*singeRow] == 0u);
			}
			if (rowDifference == -1)
			{
				return ((*ctx.board)[ctx.move->source - singeRow] == 0u);
			}
		}
	}
	else
	{
		auto collumnDiff = ctx.sourceColumn - ctx.targetColumn;
		if (collumnDiff != 1 and collumnDiff != -1)
		{
			return false;
		}
		const auto& targetField = (*ctx.board)[ctx.move->destination];
		const auto& prevMove = ctx.board->lastMove;
		if (ctx.pieceColor == NOTATION::COLOR::color::white)
		{
			return rowDifference == 1
				and ((targetField != 0u and
					NotationConversions::getPieceColor(targetField) == NOTATION::COLOR::color::black)
				or ((*ctx.board)[prevMove.destination] == (NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN)
					and prevMove.source == prevMove.destination + 2 * singeRow));
		}
		if (ctx.pieceColor == NOTATION::COLOR::color::black)
		{
			return rowDifference == -1
				and ((targetField != 0u and
					NotationConversions::getPieceColor(targetField) == NOTATION::COLOR::color::white)
				or ((*ctx.board)[prevMove.destination] == (NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN)
					and prevMove.source == prevMove.destination - 2 * singeRow));
		}
	}

	return false;
}

signed char sign (signed char a)
{
	if (a < 0)
		return -1;
	if (a > 0)
		return 1;
	return 0;
}

bool validateRock()
{
	auto isOverVertical = ctx.sourceColumn == ctx.targetColumn;
	auto isOverHorizontal = ctx.sourceRow == ctx.targetRow;

	if (isOverHorizontal)
	{
		auto r =  ctx.sourceRow;
		auto s = sign(ctx.targetColumn - ctx.sourceColumn);
		for (auto c = ctx.sourceColumn + s; c != ctx.targetColumn; c+s)
		{
			if ((*ctx.board)[NotationConversions::getFieldNum(r, c)] != 0)
				return false;
		}
		return true;
	}
	if (isOverVertical)
	{
		auto c =  ctx.sourceColumn;
		auto s = sign(ctx.targetRow - ctx.sourceRow);
		for (auto r = ctx.sourceRow + s; r != ctx.targetColumn; r+s)
		{
			if ((*ctx.board)[NotationConversions::getFieldNum(r, c)] != 0)
				return false;
		}
		return true;
	}
	return false;
}

bool validateBishop()
{
	auto isToUperRightDiagonal = (ctx.sourceColumn - ctx.sourceRow)
			== (ctx.targetColumn + ctx.targetRow);
	auto isToBotopRightDiagonal = (ctx.sourceColumn + ctx.sourceRow)
			== (ctx.targetColumn + ctx.targetRow);

	if (isToUperRightDiagonal or isToBotopRightDiagonal)
	{
		auto s_c = sign(ctx.targetColumn - ctx.sourceColumn);
		auto s_r = sign(ctx.targetRow - ctx.sourceRow);

		for (unsigned char r = ctx.sourceRow + s_r,  c = ctx.sourceColumn + s_c;
			r != ctx.targetRow;
			r += s_r, c += s_c)
		{
			if ((*ctx.board)[NotationConversions::getFieldNum(r, c)] != 0)
				return false;
		}
		return true;
	}

	return false;
}

bool validateKnight()
{
	auto rowDiff = ctx.sourceRow - ctx.targetRow;
	auto colDiff = ctx.sourceColumn - ctx.targetColumn;
	auto rowDiffAbs = rowDiff * sign(rowDiff);
	auto colDiffAbs = rowDiff * sign(rowDiff);

	return (rowDiffAbs == 1 || rowDiffAbs == 2) and ((rowDiffAbs + colDiffAbs) == 3);
}

bool validateKing()
{
	auto rowDiff = ctx.sourceRow - ctx.targetRow;
	auto colDiff = ctx.sourceColumn - ctx.targetColumn;
	auto rowDiffAbs = rowDiff * sign(rowDiff);
	auto colDiffAbs = rowDiff * sign(rowDiff);

	if (colDiffAbs == 2)
	{
		auto notMovedRockPattern = NOTATION::PIECES::ROCK
			| static_cast<unsigned char>(ctx.pieceColor);
		auto hasKingMoved = (*ctx.board)[ctx.move->source]
						& NOTATION::MOVED::MOVED_MASK;
		unsigned rockColumn = 0;

		if (ctx.targetColumn > ctx.sourceColumn)
		{
			auto rockColumn = 7;
		}

		auto isCheckInBetween = [&]() -> bool {
			auto boardCopy = *ctx.board;
			auto moveCopy = *ctx.move;
			moveCopy.destination =
					moveCopy.source + sign(ctx.targetColumn - ctx.sourceColumn);
			applyMove(boardCopy, moveCopy);
			return isCheckOn(boardCopy, ctx.board->playerOnMove);
		};

		return (hasKingMoved == false)
			and (notMovedRockPattern == (*ctx.board)[
				NotationConversions::getFieldNum(ctx.sourceRow , rockColumn)])
			and not isCheckInBetween();
	}
	return (rowDiffAbs == 0 || rowDiffAbs == 1) and (colDiffAbs == 0 or colDiffAbs == 1);
}

}  // namespace

namespace MoveValidator
{

bool validateMove(const Board& board, const Move& move)
{
	ctx.board = &board;
	ctx.move = &move;
	ctx.piece = board[move.source];

	ctx.pieceColor = NotationConversions::getPieceColor(ctx.piece);

	auto isDifferentPlayerOnMove = ctx.pieceColor != board.playerOnMove;
	auto isPieceWithSameColorOnTarget = board[move.destination] != 0 and
			(ctx.pieceColor == NotationConversions::getPieceColor(board[move.destination]));
	auto isSourceAndTargetSame = move.source == move.destination;
	if (isDifferentPlayerOnMove or isPieceWithSameColorOnTarget or isSourceAndTargetSame)
	{
		return false;
	}

	ctx.sourceRow = NotationConversions::getRow(move.source);
	ctx.targetRow = NotationConversions::getRow(move.destination);
	ctx.sourceColumn = NotationConversions::getColumnNum(move.source);
	ctx.targetColumn = NotationConversions::getColumnNum(move.destination);

	auto pieceType = ctx.piece & NOTATION::PIECES::PIECES_MASK;

	return valiateConcretePiece() and noCheckAfterMove();
}

}
