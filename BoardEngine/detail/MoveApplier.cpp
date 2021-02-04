#include "MoveApplier.hpp"

#include <publicIf/NotationConversions.hpp>
#include <hashing/zobrist.hpp>

namespace
{

void applyCasltingRules(Board& board, uint64_t& positionHash, const Move& move)
{
    if (NotationConversions::getPieceType(board.getField(move.source)) == NOTATION::PIECES::KING)
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

                board.setField(rockDestination, board.getField(rockSource));
                positionHash = hash::switchField(positionHash, rockDestination, board.getField(rockSource));
                positionHash = hash::switchField(positionHash, rockSource, board.getField(rockSource));
                board.setField(rockSource, 0u);
            }
            else
            {
                auto rockSource = NotationConversions::getFieldNum(row, COLUMN::H);
                auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::F);

                board.setField(rockDestination, board.getField(rockSource));
                positionHash = hash::switchField(positionHash, rockDestination, board.getField(rockDestination));
                positionHash = hash::switchField(positionHash, rockSource, board.getField(rockSource));
                board.setField(rockSource, 0u);
            }
        }
    }
}

void conditionallyRevokeCastlingRights(Board& board, uint64_t& positionHash, const Move& move)
{
    switch (move.source)
    {
        case 0:
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::WHITE_LONG_BIT);
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            return;
        case 4:
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::WHITE_LONG_BIT);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT);
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            return;
        case 7:
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT);
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            return;
        case 56:
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            return;
        case 60:
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            return;
        case 63:
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            board.castlingRights = board.castlingRights & (~NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
            positionHash = hash::switchCastlingRights(positionHash, board.castlingRights);
            return;
    }
}

void applyPromotionRules(Board& board, uint64_t& positionHash, const Move& move)
{
    if (move.isPromoted)
    {
        positionHash = hash::switchField(positionHash, move.destination, board.getField(move.destination));
        board.setField(move.destination, static_cast<unsigned char>(board.playerOnMove) |
                                  (move.promoteTo & NOTATION::PIECES::PIECES_MASK));
        positionHash = hash::switchField(positionHash, move.destination, board.getField(move.destination));
    }
}

unsigned char abs(signed char num)
{
    return (num > 0) ? num : -1 * num;
}

void applyEnPassantRules(Board& board, uint64_t& positionHash, const Move& move)
{
    if (NotationConversions::getPieceType(board.getField(move.source) == NOTATION::PIECES::PAWN
        and move.destination == board.validEnPassant))
    {
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF + 1)
        {
            auto fieldNum = move.source + (move.source > move.destination ? -1 : 1);
            positionHash = hash::switchField(positionHash, fieldNum, board.getField(fieldNum));
            board.setField(fieldNum, 0);
        }
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF - 1)
        {
            auto fieldNum = move.source + (move.source > move.destination ? 1 : -1);
            positionHash = hash::switchField(positionHash, fieldNum, board.getField(fieldNum));
            board.setField(fieldNum, 0);
        }
        positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
        board.validEnPassant = -1;
        return;
    }
    if (NotationConversions::getPieceType(board.getField(move.source) == NOTATION::PIECES::PAWN))
    {
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == 2 * NOTATION::COORDINATES::ROW_DIFF)
        {
            auto col = NotationConversions::getColumnNum(move.destination);
            if (
                (col < 7
                    && ((board.getField(move.destination + 1))
                        == (NOTATION::PIECES::PAWN | (~board.getField(move.source) & NOTATION::COLOR::COLOR_MASK))))
                or
                (col > 0
                    && ((board.getField(move.destination - 1))
                        == (NOTATION::PIECES::PAWN | (~board.getField(move.source) & NOTATION::COLOR::COLOR_MASK)))))
            {
                positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
                board.validEnPassant = (move.source + move.destination) / 2;
                positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
            }
            else
            {
                positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
                board.validEnPassant = -1;
            }
            return;
        }
    }

    positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
    board.validEnPassant = -1;
}

}  // namespace

namespace MoveApplier
{

void applyMove(Board& board, const Move& move)
{
    uint64_t nop;
    applyMove(board, nop, move);
}

void applyMove(Board& board, uint64_t& positionHash, const Move& move)
{
    applyCasltingRules(board, positionHash, move);
    conditionallyRevokeCastlingRights(board, positionHash, move);
    applyEnPassantRules(board, positionHash, move);

    positionHash = hash::switchField(positionHash, move.destination, board.getField(move.destination));
    board.setField(move.destination, board.getField(move.source));
    positionHash = hash::switchField(positionHash, move.destination, board.getField(move.destination));

    positionHash = hash::switchField(positionHash, move.source, board.getField(move.source));
    board.setField(move.source, 0);
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
