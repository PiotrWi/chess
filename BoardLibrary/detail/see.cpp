#include "sse.hpp"

int seeImpl(Board board, NOTATION::COLOR::color playerColor, unsigned char field, unsigned char pieceOnField);

int see(ExtendedMove& move, Board board, NOTATION::COLOR::color playerColor)
{
    MoveApplier::applyMove(board, move);
    return PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, move.destination, move.sourcePiece);
}

uint64_t getBlackPawnsAttackingField(const BitBoardsConstants& lookup)
{
    return lookup.OppositePawnsAttackingFieldForWhite;
}

uint64_t getwhitePawnsAttackingField(const BitBoardsConstants& lookup)
{
    return lookup.OppositePawnsAttackingFieldForBlack;
}

uint64_t (*getPawnsAttackingFields[2])(const BitBoardsConstants& lookup) = {getwhitePawnsAttackingField, getBlackPawnsAttackingField};

int seeImpl(Board board, NOTATION::COLOR::color playerColor, unsigned char field, unsigned char pieceOnField)
{
    const auto& lookup = bitBoardLookup[field];

    auto allPawns = board.piecesBitSets[static_cast<unsigned char>(playerColor)].pawnsMask;
    auto pawnsAttacking = allPawns & getPawnsAttackingFields[static_cast<unsigned char>(playerColor)](lookup);
    if (pawnsAttacking)
    {
        unsigned char pawnOnMove = NOTATION::PIECES::PAWN | static_cast<unsigned char>(playerColor);
        auto move = ExtendedMove (getOccupiedBitIndex(pawnsAttacking), field, ExtendedMove::beatingMask | ExtendedMove::pawnMoveMask, 0, pawnOnMove, pieceOnField);
        MoveApplier::applyMove(board, move);
        return std::max(0, PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, field, pawnOnMove));
    }

    auto attackingKnights = board.piecesBitSets[static_cast<unsigned char>(playerColor)].knightsMask
        & lookup.knightsMovePossibilities;
    if (attackingKnights)
    {
        unsigned char knightOnMove = NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(playerColor);
        auto move = ExtendedMove (getOccupiedBitIndex(attackingKnights), field, ExtendedMove::beatingMask, 0, knightOnMove, pieceOnField);
        MoveApplier::applyMove(board, move);
        return std::max(0, PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, field, knightOnMove));
    }

    auto allPieces = getAllOccupiedFields(board);
    uint64_t bishopAttackers = bishopMagicBb.getAttacksFor(field, allPieces) & board.piecesBitSets[static_cast<unsigned char>(playerColor)].bishopsMask;
    if (bishopAttackers)
    {
        unsigned char bishopOnMOve = NOTATION::PIECES::BISHOP | static_cast<unsigned char>(playerColor);
        auto move = ExtendedMove (getOccupiedBitIndex(bishopAttackers), field, ExtendedMove::beatingMask, 0, bishopOnMOve, pieceOnField);
        MoveApplier::applyMove(board, move);
        return std::max(0, PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, field, bishopOnMOve));
    }

    uint64_t rockAttackers = rockMagicBb.getAttacksFor(field, allPieces) & board.piecesBitSets[static_cast<unsigned char>(playerColor)].rocksMask;
    if (rockAttackers)
    {
        unsigned char rockOnMOve = NOTATION::PIECES::ROCK | static_cast<unsigned char>(playerColor);
        auto move = ExtendedMove (getOccupiedBitIndex(rockAttackers), field, ExtendedMove::beatingMask, 0, rockOnMOve, pieceOnField);
        MoveApplier::applyMove(board, move);
        return std::max(0, PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, field, rockOnMOve));
    }

    uint64_t queenAttackers = (bishopMagicBb.getAttacksFor(field, allPieces) | rockMagicBb.getAttacksFor(field, allPieces)) 
        & board.piecesBitSets[static_cast<unsigned char>(playerColor)].queensMask;
    if (queenAttackers)
    {
        unsigned char queenOnMOve = NOTATION::PIECES::QUEEN | static_cast<unsigned char>(playerColor);
        auto move = ExtendedMove (getOccupiedBitIndex(queenAttackers), field, ExtendedMove::beatingMask, 0, queenOnMOve, pieceOnField);
        MoveApplier::applyMove(board, move);
        return std::max(0, PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, field, queenOnMOve));
    }

    auto attackingKings = board.piecesBitSets[static_cast<unsigned char>(playerColor)].kingsMask & lookup.kingMovePossibilities;
    if (attackingKings)
    {
        unsigned char kingOnMove = NOTATION::PIECES::KING | static_cast<unsigned char>(playerColor);
        auto move = ExtendedMove (getOccupiedBitIndex(attackingKings), field, ExtendedMove::beatingMask | ExtendedMove::kingMoveMask, 0, kingOnMove, pieceOnField);
        MoveApplier::applyMove(board, move);
        return std::max(0, PiecesValuesTable[move.targetPiece] - seeImpl(board, ++playerColor, field, kingOnMove));
    }

    return 0;
}