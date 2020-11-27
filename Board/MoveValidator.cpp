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

bool validatePawn();

bool valiateConcretePiece()
{
	auto pieceType = ctx.piece & NOTATION::PIECES::PIECES_MASK;

	if (pieceType == NOTATION::PIECES::PAWN)
	{
		return validatePawn();
	}
	if (pieceType == NOTATION::PIECES::ROCK)
	{
		// isVertical and noPieceinBeetween and targetEmptyOrEnemy
		// or isHorizontall and noPieceinBeetween and targetEmptyOrEnemy
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

	if (isDifferentPlayerOnMove or isPieceWithSameColorOnTarget)
	{
		return false;
	}

	ctx.sourceRow = NotationConversions::getRow(move.source);
	ctx.targetRow = NotationConversions::getRow(move.destination);
	ctx.sourceColumn = NotationConversions::getColumnNum(move.source);
	ctx.targetColumn = NotationConversions::getColumnNum(move.destination);

	auto pieceType = ctx.piece & NOTATION::PIECES::PIECES_MASK;

	return valiateConcretePiece() and noCheckAfterMove();
	if (pieceType == NOTATION::PIECES::PAWN)
	{

		// is2RowsMove or isSingleRowMove or isBearingInFly or isBeatig or isInLastLineWithPromotion;
	}
	if (pieceType == NOTATION::PIECES::ROCK)
	{
		// isVertical and noPieceinBeetween and targetEmptyOrEnemy
		// or isHorizontall and noPieceinBeetween and targetEmptyOrEnemy
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
	throw 1; // Not implemented yet
	return true;
}

}
