#include "MoveValidator.hpp"

#include "core/Board.hpp"
#include "MoveApplier/MoveApplier.hpp"
#include "core/NotationConversions.hpp"
#include "PositionAnalyzers/CheckChecker.hpp"

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

	switch(pieceType)
	{
	case(NOTATION::PIECES::PAWN): return validatePawn();
	case(NOTATION::PIECES::ROCK): return validateRock();
	case(NOTATION::PIECES::BISHOP): return validateBishop();
	case(NOTATION::PIECES::KNIGHT): return validateKnight();
	case(NOTATION::PIECES::QUEEN): return validateRock() or validateBishop();
	case(NOTATION::PIECES::KING): return validateKing();
	}

	return false;
}

bool noCheckAfterMove()
{
	Board boardCopy = *ctx.board;
	MoveApplier::applyMove(boardCopy, convertMoveToExtended(boardCopy, *ctx.move));
    return not CheckChecker::isCheckOn(boardCopy, ctx.board->playerOnMove);
}

bool validatePawn()
{
	constexpr unsigned char singeRow = NOTATION::COORDINATES::ROW_DIFF;
    auto isBeating = (ctx.sourceColumn != ctx.targetColumn);
	auto rowDifference = ctx.targetRow - ctx.sourceRow;

	if (not isBeating)
	{
		if (ctx.pieceColor == NOTATION::COLOR::color::white)
		{
			if (rowDifference == 2)
            {
				return ctx.sourceRow == 1
					and ((*ctx.board).getField(ctx.move->source + singeRow) == 0u)
					and ((*ctx.board).getField(ctx.move->source + 2 * singeRow) == 0u);
			}
			if (rowDifference == 1
				and ctx.pieceColor == NOTATION::COLOR::color::white)
			{
				return ((*ctx.board).getField(ctx.move->source + singeRow) == 0u);
			}
		}
		else
		{
			if (rowDifference == -2)
			{
				return ctx.sourceRow == 6
					and ((*ctx.board).getField(ctx.move->source - singeRow) == 0u)
					and ((*ctx.board).getField(ctx.move->source - 2*singeRow) == 0u);
			}
			if (rowDifference == -1)
			{
				return ((*ctx.board).getField(ctx.move->source - singeRow) == 0u);
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
        if (ctx.pieceColor == NOTATION::COLOR::color::white)
        {
            if (rowDifference == 1)
            {
                if (ctx.move->destination == ctx.board->validEnPassant or
                (ctx.board->getField(ctx.move->destination) & NOTATION::COLOR::COLOR_MASK) == NOTATION::COLOR::BLACK)
                {
                    return true;
                }
            }
        }
        if (ctx.pieceColor == NOTATION::COLOR::color::black)
        {
            if (rowDifference == -1)
            {
                if (ctx.move->destination == ctx.board->validEnPassant or
                    (ctx.board->getField(ctx.move->destination) != 0 and
                    (ctx.board->getField(ctx.move->destination) & NOTATION::COLOR::COLOR_MASK) == NOTATION::COLOR::WHITE))
                {
                    return true;
                }
            }
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
        for (auto c = ctx.sourceColumn + s; c != ctx.targetColumn; c+=s)
		{
			if (ctx.board->getField(NotationConversions::getFieldNum(r, c)) != 0)
				return false;
		}
		return true; /*Target field color check is done in parrent fcn*/
	}
	if (isOverVertical)
	{
		auto c =  ctx.sourceColumn;
		auto s = sign(ctx.targetRow - ctx.sourceRow);
        for (auto r = ctx.sourceRow + s; r != ctx.targetRow; r+=s)
		{
			if (ctx.board->getField(NotationConversions::getFieldNum(r, c)) != 0)
				return false;
		}
		return true; /*Target field color check is done in parrent fcn*/
	}
	return false;
}

bool validateBishop()
{
	auto isToUperRightDiagonal = (ctx.sourceColumn - ctx.sourceRow)
			== (ctx.targetColumn - ctx.targetRow);
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
			if (ctx.board->getField(NotationConversions::getFieldNum(r, c)) != 0)
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
    auto colDiffAbs = colDiff * sign(colDiff);

	return (rowDiffAbs == 1 || rowDiffAbs == 2) and ((rowDiffAbs + colDiffAbs) == 3);
}

bool validateKing()
{
	auto rowDiff = ctx.sourceRow - ctx.targetRow;
	auto colDiff = ctx.sourceColumn - ctx.targetColumn;
	auto rowDiffAbs = rowDiff * sign(rowDiff);
    auto colDiffAbs = colDiff * sign(colDiff);

	if (colDiffAbs == 2)
	{
		unsigned rockColumn = 0;

		if (ctx.targetColumn > ctx.sourceColumn)
		{
            rockColumn = 7;
		}

		auto hasRights = [&]() -> bool {
		    if (ctx.pieceColor == NOTATION::COLOR::color::white)
            {
		        if (rockColumn == 0)
                {
                    return ctx.board->castlingRights & NOTATION::CASTLING_RIGHTS::WHITE_LONG_BIT;
                }
                return ctx.board->castlingRights & NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT;
            }
            if (rockColumn == 0)
            {
                return ctx.board->castlingRights & NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT;
            }
            return ctx.board->castlingRights & NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT;
		};
		auto isCheckInBetween = [&]() -> bool {
			auto boardCopy = *ctx.board;
			auto moveCopy = convertMoveToExtended(boardCopy, *ctx.move);
			moveCopy.destination =
					moveCopy.source + sign(ctx.targetColumn - ctx.sourceColumn);
			MoveApplier::applyMove(boardCopy, moveCopy);
			return CheckChecker::isCheckOn(boardCopy, ctx.board->playerOnMove);
		};

		return hasRights() and not isCheckInBetween();
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
	ctx.piece = board.getField(move.source);

	ctx.pieceColor = NotationConversions::getPieceColor(ctx.piece);

	auto isDifferentPlayerOnMove = ctx.pieceColor != board.playerOnMove;
	auto isPieceWithSameColorOnTarget = board.getField(move.destination) != 0 and
			(ctx.pieceColor == NotationConversions::getPieceColor(board.getField(move.destination)));
	auto isSourceAndTargetSame = move.source == move.destination;
	if (isDifferentPlayerOnMove or isPieceWithSameColorOnTarget or isSourceAndTargetSame)
	{
		return false;
	}

	ctx.sourceRow = NotationConversions::getRow(move.source);
	ctx.targetRow = NotationConversions::getRow(move.destination);
	ctx.sourceColumn = NotationConversions::getColumnNum(move.source);
	ctx.targetColumn = NotationConversions::getColumnNum(move.destination);

	return valiateConcretePiece() and noCheckAfterMove();
}

}
