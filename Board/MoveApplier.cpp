#include "MoveApplier.hpp"

#include <NotationConversions.hpp>
#include <ResultEvaluator.hpp>

namespace
{

void applyCasltingRules(Board& board, const Move& move)
{
    if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::KING)
    {
        using namespace NOTATION::COORDINATES;
        const auto sourceColumn = NotationConversions::getColumnNum(move.source);
        const auto targetColumn = NotationConversions::getColumnNum(move.destination);

        bool isCastle = sourceColumn == COLUMN::E
                        and (targetColumn == COLUMN::C
                             or targetColumn == COLUMN::G);

        if (isCastle)
        {
            auto row = NotationConversions::getRow(move.source);
            bool isLongCastle = targetColumn == COLUMN::C;
            if (isLongCastle)
            {
                auto rockSource = NotationConversions::getFieldNum(row, COLUMN::A);
                auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::D);
                board[rockDestination] = board[rockSource];
                board[rockSource] = 0u;
            }
            else
            {
                auto rockSource = NotationConversions::getFieldNum(row, COLUMN::A);
                auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::D);
                board[rockDestination] = board[rockSource];
                board[rockSource] = 0u;

            }
        }
    }
}

void applyPromotionRules(Board& board, const Move& move)
{
    if (move.isPromoted)
    {
        board[move.destination] = static_cast<unsigned char>(board.playerOnMove) |
                                  (move.promoteTo & NOTATION::PIECES::PIECES_MASK);
    }
}

unsigned char abs(signed char num)
{
    return (num > 0) ? num : -1 * num;
}

void applyEnPassantRules(Board& board, const Move& move)
{
    if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::PAWN
        and board[move.destination] == 0)
    {
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF + 1)
        {
            board[move.destination+1] = 0;
            return;
        }
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF - 1)
        {
            board[move.destination-1] = 0;
            return;
        }
    }
}

}  // namespace

namespace MoveApplier
{

void applyMove(Board& board, const Move& move)
{
    board[move.destination] = board[move.source] | NOTATION::MOVED::MOVED_MASK;
    board.lastMove = move;
    board[move.source] = 0;

    applyCasltingRules(board, move);
    applyEnPassantRules(board, move);
    applyPromotionRules(board, move);

    ++board.playerOnMove;
}

}  // namespace MoveApplier
