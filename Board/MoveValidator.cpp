#include "MoveValidator.hpp"

#include <Board.hpp>
#include <NotationConversions.hpp>

namespace MoveValidator
{

bool validateMove(const Board& board, const Move& move)
{
	auto piece = board.fields[move.source];
	auto pieceColor = NotationConversions::getPieceColor(piece);
	auto piecetype = NotationConversions::getPieceType(piece);

	auto isSourceFieldEmpty = (piece == 0u);
	auto isDifferentPlayerOnMove = pieceColor != board.playerOnMove;
	auto targetFieldWithSameColor = false;

	auto sourceRow = NotationConversions::getRow(move.source);
	auto targetRow = NotationConversions::getRow(move.destination);

	auto sourceColumn = NotationConversions::getColumnNum(move.source);
	auto targetColumn = NotationConversions::getColumnNum(move.destination);

	if (isSourceFieldEmpty or isDifferentPlayerOnMove or targetFieldWithSameColor)
	{
		return false;
	}

	if (piecetype == NOTATION::PIECES::PAWN)
	{
		// is2RowsMove or isSingleRowMove or isBearingInFly or isBeatig or isInLastLineWithPromotion;
	}
	if (piecetype == NOTATION::PIECES::ROCK)
	{
		// isVertical and noPieceinBeetween and targetEmptyOrEnemy
		// or isHorizontall and noPieceinBeetween and targetEmptyOrEnemy
	}
	if (piecetype == NOTATION::PIECES::KNIGHT)
	{

	}
	if (piecetype == NOTATION::PIECES::BISHOP)
	{

	}
	if (piecetype == NOTATION::PIECES::QUEEN)
	{

	}
	if (piecetype == NOTATION::PIECES::KING)
	{

	}
	throw 1; // Not implemented yet
	return true;
}

}
