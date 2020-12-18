#include "MoveApplier.hpp"

#include <NotationConversions.hpp>

namespace
{

void applyCasltingRules(Board& board, const Move& move)
{
    if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::KING)
    {
        bool isCastle = NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::E
                        and (NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::C
                             or NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::G);

        if (isCastle)
        {
            auto row = NotationConversions::getRow(move.source);
            bool isLongCastle = NotationConversions::getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::C;
            if (isLongCastle)
            {
                auto rockSource = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::A);
                auto rockDestination = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::D);
                board[rockDestination] = board[rockSource];
                board[rockSource] = 0u;
            }
            else
            {
                auto rockSource = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::A);
                auto rockDestination = NotationConversions::getFieldNum(row, NOTATION::COORDINATES::COLUMN::D);
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
