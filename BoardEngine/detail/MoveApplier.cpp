#include "MoveApplier.hpp"

#include <publicIf/NotationConversions.hpp>
#include <hashing/zobrist.hpp>

namespace
{

bool applyCasltingRules(Board& board, uint64_t& positionHash, const Move& move)
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

                board[rockDestination] = board[rockSource] | NOTATION::MOVED::MOVED_MASK;
                hash::switchField(positionHash, rockDestination, board[rockDestination]);
                hash::switchField(positionHash, board[rockSource], board[rockDestination]);
                board[rockSource] = 0u;

            }
            else
            {
                auto rockSource = NotationConversions::getFieldNum(row, COLUMN::H);
                auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::F);

                board[rockDestination] = board[rockSource] | NOTATION::MOVED::MOVED_MASK;
                hash::switchField(positionHash, rockDestination, board[rockDestination]);
                hash::switchField(positionHash, board[rockSource], board[rockDestination]);
                board[rockSource] = 0u;
            }
            return true;
        }
    }
    return false;
}

void applyPromotionRules(Board& board, uint64_t& positionHash, const Move& move)
{
    if (move.isPromoted)
    {
        hash::switchField(positionHash, move.destination, board[move.destination]);
        board[move.destination] = static_cast<unsigned char>(board.playerOnMove) |
                                  (move.promoteTo & NOTATION::PIECES::PIECES_MASK) |
                                  NOTATION::MOVED::MOVED_MASK;
        hash::switchField(positionHash, move.destination, board[move.destination]);
    }
}

unsigned char abs(signed char num)
{
    return (num > 0) ? num : -1 * num;
}

bool applyEnPassantRules(Board& board, uint64_t& positionHash, const Move& move)
{
    if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::PAWN
        and move.destination == board.validEnPassant)
    {
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF + 1)
        {
            auto fieldNum = move.source + (move.source > move.destination ? -1 : 1);
            positionHash = hash::switchField(positionHash, fieldNum, board[fieldNum]);
            board[fieldNum] = 0;
        }
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF - 1)
        {
            auto fieldNum = move.source + (move.source > move.destination ? 1 : -1);
            positionHash = hash::switchField(positionHash, fieldNum, board[fieldNum]);
            board[fieldNum] = 0;
        }
        positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
        board.validEnPassant = -1;
        return true;
    }
    if (NotationConversions::getPieceType(board[move.source]) == NOTATION::PIECES::PAWN)
    {
        // TODO Condition can be further detailed to set this field less often.
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == 2 * NOTATION::COORDINATES::ROW_DIFF)
        {
            positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
            board.validEnPassant = (move.source + move.destination) / 2;
            positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);

            return false;
        }
    }

    positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
    board.validEnPassant = -1;
    return false;
}

}  // namespace

namespace MoveApplier
{

MoveMemorial applyTmpMove(Board& board, uint64_t& positionHash, const Move& move)
{
    MoveMemorial moveMemorial;

    moveMemorial.positionHash = positionHash;
    moveMemorial.sourceField = move.source;
    moveMemorial.targetField = move.destination;

    moveMemorial.sourceVal = board[move.source];
    moveMemorial.targetVal = board[move.destination];

    moveMemorial.wasCasling = applyCasltingRules(board, positionHash, move);
    moveMemorial.enPasant = applyEnPassantRules(board, positionHash, move);

    positionHash = hash::switchField(positionHash, move.destination, board[move.destination]);
    board[move.destination] = board[move.source] | NOTATION::MOVED::MOVED_MASK;
    positionHash = hash::switchField(positionHash, move.destination, board[move.destination]);

    positionHash = hash::switchField(positionHash, move.source, board[move.source]);
    board[move.source] = 0;
    applyPromotionRules(board, positionHash, move);

    positionHash = hash::switchColor(positionHash);
    ++board.playerOnMove;
    return moveMemorial;
}

void undoMove(Board& board, uint64_t& positionHash, const MoveMemorial& memorial)
{
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

    positionHash = memorial.positionHash;
}

void applyMove(Board& board, const Move& move)
{
    uint64_t nop;
    applyMove(board, nop, move);
}


void applyMove(Board& board, uint64_t& positionHash, const Move& move)
{
    applyCasltingRules(board, positionHash, move);
    applyEnPassantRules(board, positionHash, move);

    positionHash = hash::switchField(positionHash, move.destination, board[move.destination]);
    board[move.destination] = board[move.source] | NOTATION::MOVED::MOVED_MASK;
    positionHash = hash::switchField(positionHash, move.destination, board[move.destination]);

    positionHash = hash::switchField(positionHash, move.source, board[move.source]);
    board[move.source] = 0;
    applyPromotionRules(board, positionHash, move);

    positionHash = hash::switchColor(positionHash);
    ++board.playerOnMove;
}

SimpleMoveMemorial applyTmpMoveSimple(Board& board, uint64_t& positionHash, const Move& move)
{
    SimpleMoveMemorial memorial{board, positionHash};
    applyMove(board, positionHash, move);
    return memorial;
}

void undoMove(Board& board, uint64_t& positionHash, const SimpleMoveMemorial& memorial)
{
    board = memorial.previousBoard;
    positionHash = memorial.positionHash;
}

}  // namespace MoveApplier
