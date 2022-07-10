#pragma once
#include <publicIf/Board.hpp>

namespace MoveGenerator
{

thread_local struct MoveContext{
    Board* board;
    NOTATION::COLOR::color pieceColor;
    unsigned char kingPosition;

    unsigned Nbeatings;
    ExtendedMove beatings[100];
    unsigned Npromotions;
    ExtendedMove promotions[120];
    unsigned NFiguresMoves;
    ExtendedMove figuresMoves[100];
    unsigned NPawnsMoves;
    ExtendedMove pawnsMoves[40];
    unsigned NKingMoves;
    ExtendedMove kingMoves[11];

    uint64_t allPieces;
    uint64_t opponentPieces;
} ctx;

}  // namespace MoveGenerator
