#include "MoveApplier.hpp"

#include <publicIf/NotationConversions.hpp>
#include <hashing/zobrist.hpp>

namespace
{

void applyCasltingRules(Board& board, uint64_t& positionHash, const ExtendedMove& move)
{
    using namespace NOTATION::COORDINATES;
    const auto targetColumn = NotationConversions::getColumnNum(move.destination);
    bool isLongCastle = targetColumn == COLUMN::C;
    auto color = move.sourcePiece & NOTATION::COLOR::COLOR_MASK;
    auto row = NotationConversions::getRow(move.source);

    if (isLongCastle)
    {
        auto rockSource = NotationConversions::getFieldNum(row, COLUMN::A);
        auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::D);

        board.setPreviouslyEmptyField(rockDestination, NOTATION::PIECES::ROCK | color);
        positionHash = hash::switchField(positionHash, rockDestination, NOTATION::PIECES::ROCK | color);
        positionHash = hash::switchField(positionHash, rockSource, NOTATION::PIECES::ROCK | color);
        board.clearField(rockSource, NOTATION::PIECES::ROCK | color);
    }
    else
    {
        auto rockSource = NotationConversions::getFieldNum(row, COLUMN::H);
        auto rockDestination = NotationConversions::getFieldNum(row, COLUMN::F);

        board.setPreviouslyEmptyField(rockDestination, NOTATION::PIECES::ROCK | color);
        positionHash = hash::switchField(positionHash, rockDestination, NOTATION::PIECES::ROCK | color);
        positionHash = hash::switchField(positionHash, rockSource, NOTATION::PIECES::ROCK | color);
        board.clearField(rockSource, NOTATION::PIECES::ROCK | color);
    }

    board.setPreviouslyEmptyField(move.destination, NOTATION::PIECES::KING | color);
    positionHash = hash::switchField(positionHash, move.destination, NOTATION::PIECES::KING | color);
    positionHash = hash::switchField(positionHash, move.source, NOTATION::PIECES::KING | color);
    board.clearField(move.source, NOTATION::PIECES::KING | color);
}

void revokeCastlingRights(Board& board, uint64_t& positionHash, const Move& move)
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

void applyPromotionRules(Board& board, uint64_t& positionHash, const ExtendedMove& move)
{
    if (move.flags & ExtendedMove::promotionMask)
    {
        positionHash = hash::switchField(positionHash, move.destination, move.sourcePiece);
        board.setField(move.destination, move.sourcePiece,static_cast<unsigned char>(board.playerOnMove) |
                                  (move.promoting & NOTATION::PIECES::PIECES_MASK));
        positionHash = hash::switchField(positionHash,
                                         move.destination,
                                         static_cast<unsigned char>(board.playerOnMove) |
                                             (move.promoting & NOTATION::PIECES::PIECES_MASK));
    }
}

unsigned char abs(signed char num)
{
    return (num > 0) ? num : -1 * num;
}

void switchOffEnPassant(Board& board, uint64_t& positionHash)
{
    positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
    board.validEnPassant = -1;
}

void applyEnPassantRules(Board& board, uint64_t& positionHash, const ExtendedMove& move)
{
    if (move.flags & ExtendedMove::enPasantMask)
    {
        auto moveDiff = move.source-move.destination;
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF + 1)
        {
            auto fieldNum = move.source + (move.source > move.destination ? -1 : 1);
            positionHash = hash::switchField(positionHash, fieldNum, move.targetPiece);
            board.clearField(fieldNum, move.targetPiece);
        }
        if (abs(moveDiff) == NOTATION::COORDINATES::ROW_DIFF - 1)
        {
            auto fieldNum = move.source + (move.source > move.destination ? 1 : -1);
            positionHash = hash::switchField(positionHash, fieldNum, move.targetPiece);
            board.clearField(fieldNum, move.targetPiece);
        }
        switchOffEnPassant(board, positionHash);
        return;
    }

    auto moveDiff = move.source-move.destination;
    if (abs(moveDiff) == 2 * NOTATION::COORDINATES::ROW_DIFF)
    {
        auto col = NotationConversions::getColumnNum(move.destination);
        if (
            (col < 7
                && ((board.getField(move.destination + 1))
                    == (NOTATION::PIECES::PAWN | static_cast<unsigned char>(board.playerOnMove + 1))))
            or
            (col > 0
                && ((board.getField(move.destination - 1))
                    == (NOTATION::PIECES::PAWN | static_cast<unsigned char>(board.playerOnMove + 1)))))
        {
            positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
            board.validEnPassant = (move.source + move.destination) / 2;
            positionHash = hash::switchEnPassant(positionHash, board.validEnPassant);
        }
        else
        {
            switchOffEnPassant(board, positionHash);
        }
        return;
    }
    switchOffEnPassant(board, positionHash);
}



}  // namespace

namespace MoveApplier
{

void applyMove(Board& board, const ExtendedMove& move)
{
    uint64_t nop;
    applyMove(board, nop, move);
}

void applyMove(Board& board, uint64_t& positionHash, const ExtendedMove& move)
{
    if (move.flags & ExtendedMove::castlingMask)
    {
        applyCasltingRules(board, positionHash, move);
        revokeCastlingRights(board, positionHash, move);
        switchOffEnPassant(board, positionHash);
        ++board.playerOnMove;
        positionHash = hash::switchColor(positionHash);
        return;
    }
    if (move.flags & ExtendedMove::pawnMoveMask)
    {
        applyEnPassantRules(board, positionHash, move);
        if ((move.flags & ExtendedMove::beatingMask) && !(move.flags & ExtendedMove::enPasantMask))
        {
            positionHash = hash::switchField(positionHash, move.destination, move.targetPiece);
            board.clearField(move.destination, move.targetPiece);
        }
        board.setPreviouslyEmptyField(move.destination, move.sourcePiece);
        positionHash = hash::switchField(positionHash, move.destination, move.sourcePiece);

        positionHash = hash::switchField(positionHash, move.source, move.sourcePiece);
        board.clearField(move.source, move.sourcePiece);

        applyPromotionRules(board, positionHash, move);

        ++board.playerOnMove;
        positionHash = hash::switchColor(positionHash);
        return;
    }

    if (move.flags & (ExtendedMove::rockMoveMask | ExtendedMove::kingMoveMask))
    {
        revokeCastlingRights(board, positionHash, move);
    }
    if (move.flags & ExtendedMove::beatingMask)
    {
        positionHash = hash::switchField(positionHash, move.destination, move.targetPiece);
        board.clearField(move.destination, move.targetPiece);
    }
    board.setPreviouslyEmptyField(move.destination, move.sourcePiece);
    positionHash = hash::switchField(positionHash, move.destination, move.sourcePiece);

    positionHash = hash::switchField(positionHash, move.source, move.sourcePiece);
    board.clearField(move.source, move.sourcePiece);

    switchOffEnPassant(board, positionHash);
    positionHash = hash::switchColor(positionHash);
    ++board.playerOnMove;
}

SimpleMoveMemorial applyTmpMoveSimple(Board& board, uint64_t& positionHash, const ExtendedMove& move)
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
