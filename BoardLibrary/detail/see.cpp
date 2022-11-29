#include "sse.hpp"

template<NOTATION::COLOR::color playerColor>
int seeImpl(Board& board, unsigned char field, unsigned char pieceOnField);

int see(ExtendedMove& move, Board board, NOTATION::COLOR::color playerColor)
{
    board.clearField(move.source, move.sourcePiece);
    if (move.flags & ExtendedMove::promotionMask)
    {
        if (++playerColor == NOTATION::COLOR::color::white)
        {
            return PiecesValuesTable[move.targetPiece] + 800 - seeImpl<NOTATION::COLOR::color::white>(board, move.destination, move.sourcePiece);
        }
        return PiecesValuesTable[move.targetPiece] + 800 - seeImpl<NOTATION::COLOR::color::black>(board, move.destination, move.sourcePiece);
    }
    if (++playerColor == NOTATION::COLOR::color::white)
    {
        return PiecesValuesTable[move.targetPiece] - seeImpl<NOTATION::COLOR::color::white>(board, move.destination, move.sourcePiece);
    }
    return PiecesValuesTable[move.targetPiece] - seeImpl<NOTATION::COLOR::color::black>(board, move.destination, move.sourcePiece);
}

template<NOTATION::COLOR::color playerColor>
uint64_t getPawnsAttackingFields(const BitBoardsConstants& lookup);

template<>
uint64_t getPawnsAttackingFields<NOTATION::COLOR::color::black>(const BitBoardsConstants& lookup)
{
    return lookup.OppositePawnsAttackingFieldForWhite;
}

template<>
uint64_t getPawnsAttackingFields<NOTATION::COLOR::color::white>(const BitBoardsConstants& lookup)
{
    return lookup.OppositePawnsAttackingFieldForBlack;
}


template<NOTATION::COLOR::color playerColor>
int seeImpl(Board& board, unsigned char field, unsigned char pieceOnField)
{
    const auto& lookup = bitBoardLookup[field];

    const auto& allPawns = board.piecesBitSets[static_cast<unsigned char>(playerColor)].pawnsMask;
    const auto& pawnsAttacking = allPawns & getPawnsAttackingFields<playerColor>(lookup);
    if (pawnsAttacking)
    {
        constexpr unsigned char pawnOnMove = NOTATION::PIECES::PAWN | static_cast<unsigned char>(playerColor);
        board.piecesBitSets[static_cast<unsigned char>(playerColor)].pawnsMask ^= (1ull << getOccupiedBitIndex(pawnsAttacking));
        return std::max(0, PiecesValuesTable[pieceOnField] - seeImpl<NotationConversions::switchColor(playerColor)>(board, field, pawnOnMove));
    }

    const auto& attackingKnights = board.piecesBitSets[static_cast<unsigned char>(playerColor)].knightsMask
        & lookup.knightsMovePossibilities;
    if (attackingKnights)
    {
        constexpr unsigned char knightOnMove = NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(playerColor);
        board.piecesBitSets[static_cast<unsigned char>(playerColor)].knightsMask ^= (1ull << getOccupiedBitIndex(attackingKnights));
        return std::max(0, PiecesValuesTable[pieceOnField] - seeImpl<NotationConversions::switchColor(playerColor)>(board, field, knightOnMove));
    }

    const auto& allPieces = getAllOccupiedFields(board);
    const uint64_t& bishopAttackers = bishopBb.getAttacksFor(field, allPieces) & board.piecesBitSets[static_cast<unsigned char>(playerColor)].bishopsMask;
    if (bishopAttackers)
    {
        constexpr unsigned char bishopOnMOve = NOTATION::PIECES::BISHOP | static_cast<unsigned char>(playerColor);
        board.piecesBitSets[static_cast<unsigned char>(playerColor)].bishopsMask ^= (1ull << getOccupiedBitIndex(bishopAttackers));
        return std::max(0, PiecesValuesTable[pieceOnField] - seeImpl<NotationConversions::switchColor(playerColor)>(board, field, bishopOnMOve));
    }

    const uint64_t& rockAttackers = rockBb.getAttacksFor(field, allPieces) & board.piecesBitSets[static_cast<unsigned char>(playerColor)].rocksMask;
    if (rockAttackers)
    {
        constexpr unsigned char rockOnMOve = NOTATION::PIECES::ROCK | static_cast<unsigned char>(playerColor);
        board.piecesBitSets[static_cast<unsigned char>(playerColor)].rocksMask ^= (1ull << getOccupiedBitIndex(rockAttackers));
        return std::max(0, PiecesValuesTable[pieceOnField] - seeImpl<NotationConversions::switchColor(playerColor)>(board, field, rockOnMOve));
    }

    const uint64_t& queenAttackers = (bishopBb.getAttacksFor(field, allPieces) | rockBb.getAttacksFor(field, allPieces)) 
        & board.piecesBitSets[static_cast<unsigned char>(playerColor)].queensMask;
    if (queenAttackers)
    {
        constexpr unsigned char queenOnMOve = NOTATION::PIECES::QUEEN | static_cast<unsigned char>(playerColor);
        board.piecesBitSets[static_cast<unsigned char>(playerColor)].queensMask ^= (1ull << getOccupiedBitIndex(queenAttackers));
        return std::max(0, PiecesValuesTable[pieceOnField] - seeImpl<NotationConversions::switchColor(playerColor)>(board, field, queenOnMOve));
    }

    const auto& attackingKings = board.piecesBitSets[static_cast<unsigned char>(playerColor)].kingsMask & lookup.kingMovePossibilities;
    if (attackingKings)
    {
        constexpr unsigned char kingOnMove = NOTATION::PIECES::KING | static_cast<unsigned char>(playerColor);
        board.piecesBitSets[static_cast<unsigned char>(playerColor)].kingsMask ^= (1ull << getOccupiedBitIndex(attackingKings));
        return std::max(0, PiecesValuesTable[pieceOnField] - seeImpl<NotationConversions::switchColor(playerColor)>(board, field, kingOnMove));
    }

    return 0;
}