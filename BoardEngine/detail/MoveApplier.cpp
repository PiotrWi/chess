#include "MoveApplier.hpp"

#include <publicIf/NotationConversions.hpp>
#include <detail/ResultEvaluator.hpp>

namespace
{

bool applyCasltingRules(Board& board, const Move& move)
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
                auto rockSource = NotationConversions::getFieldNum(row, COLUMN::H);
                auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::F);
                board[rockDestination] = board[rockSource];
                board[rockSource] = 0u;
            }
            return true;
        }
    }
    return false;
}

void applyPromotionRules(Board& board, const Move& move)
{
    if (move.isPromoted)
    {
        board[move.destination] = static_cast<unsigned char>(board.playerOnMove) |
                                  (move.promoteTo & NOTATION::PIECES::PIECES_MASK) |
                                  NOTATION::MOVED::MOVED_MASK;
    }
}

unsigned char abs(signed char num)
{
    return (num > 0) ? num : -1 * num;
}

bool applyEnPassantRules(Board& board, const Move& move)
{
    if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::PAWN
        and board[move.destination] == 0)
    {
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF + 1)
        {
            board[move.source + (move.source > move.destination ? -1 : 1)] = 0;
            return true;
        }
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF - 1)
        {
            board[move.source + (move.source > move.destination ? 1 : -1)] = 0;
            return true;
        }
    }
    return false;
}

}  // namespace

namespace MoveApplier
{

MoveMemorial applyTmpMove(Board& board, const Move& move)
{
    MoveMemorial moveMemorial;
    moveMemorial.lastMove = board.lastMove;

    moveMemorial.sourceField = move.source;
    moveMemorial.targetField = move.destination;
    moveMemorial.sourceVal = board[move.source];
    moveMemorial.targetVal = board[move.destination];

    moveMemorial.wasCasling = applyCasltingRules(board, move);
    moveMemorial.enPasant = applyEnPassantRules(board, move);

    board[move.destination] = board[move.source] | NOTATION::MOVED::MOVED_MASK;
    board.lastMove = move;
    board[move.source] = 0;
    applyPromotionRules(board, move);

    ++board.playerOnMove;
    return moveMemorial;
}

void undoMove(Board& board, const MoveMemorial& memorial)
{
    board.lastMove = memorial.lastMove;
    ++board.playerOnMove;

    board[memorial.sourceField] = memorial.sourceVal;
    board[memorial.targetField] = memorial.targetVal;

    if (memorial.wasCasling)
    {
        if (memorial.sourceField > memorial.targetField)
        {
            for (auto field = memorial.sourceField +1; field < memorial.targetField; ++field)
            {
                board[field] = 0;
            }
        }
        else
        {
            for (auto field = memorial.targetField +1; field < memorial.sourceField; ++field)
            {
                board[field] = 0;
            }
        }
    }
    if(memorial.enPasant)
    {
        const auto sourceColumn = NotationConversions::getColumnNum(memorial.sourceField);
        const auto targetColumn = NotationConversions::getColumnNum(memorial.targetField);

        auto oppositePawn = board[memorial.sourceField] ^ NOTATION::COLOR::COLOR_MASK;
        if (targetColumn < sourceColumn)
        {
            board[memorial.sourceField - 1] = oppositePawn;
        }
        else
        {
            board[memorial.sourceField - 1] = oppositePawn;
        }
    }
}

void applyMove(Board& board, const Move& move)
{
    applyCasltingRules(board, move);
    applyEnPassantRules(board, move);

    board[move.destination] = board[move.source] | NOTATION::MOVED::MOVED_MASK;
    board.lastMove = move;
    board[move.source] = 0;
    applyPromotionRules(board, move);

    ++board.playerOnMove;
}

}  // namespace MoveApplier
