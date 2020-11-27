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
	if (pieceType == NOTATION::PIECES::KNIGHT)
	{

	}
	if (pieceType == NOTATION::PIECES::BISHOP)
	{

	}
	if (pieceType == NOTATION::PIECES::QUEEN)
	{

	}
	if (pieceType == NOTATION::PIECES::KING)
	{

	}
	return false;
}

bool noCheckAfterMove()
{
	return true;
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
