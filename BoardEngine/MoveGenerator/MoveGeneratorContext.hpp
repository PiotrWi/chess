#pragma once
#include <publicIf/Board.hpp>

namespace MoveGenerator
{

thread_local struct MoveContext{
    const Board* board;
    NOTATION::COLOR::color pieceColor;
    unsigned char kingPosition;

    unsigned Nbeatings;
    ExtendedMove beatings[100];
    unsigned Npromotions;
    ExtendedMove promotions[100];
    unsigned NFiguresMoves;
    ExtendedMove figuresMoves[100];
    unsigned NPawnsMoves;
    ExtendedMove pawnsMoves[60];
    unsigned NKingMoves;
    ExtendedMove kingMoves[11];
} ctx;

}  // namespace MoveGenerator
