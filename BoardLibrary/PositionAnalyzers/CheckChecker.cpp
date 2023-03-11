#include "CheckChecker.hpp"

#include <algorithm>
#include "core/Board.hpp"
#include "core/NotationConversions.hpp"

#include "core/bitboards/BitBoardsUtils.h"
#include "core/bitboards/bitboardslookups.hpp"
#include "core/bitboards/FromToBits.hpp"

namespace CheckChecker
{

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);
    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;
    auto attackingPawn = allOppositePawns & getPawnAttacks(playerColor, fieldPosition);
    const auto& lookup = bitBoardLookup[fieldPosition];

    auto oppositeKing = board.piecesBitSets[oppositeColorNum].kingsMask;
    auto attackingKings = oppositeKing & lookup.kingMovePossibilities;

    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    auto attackingKnights = allOppositeKnights & lookup.knightsMovePossibilities;

    auto allPieces = getAllOccupiedFields(board);

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;

    uint64_t rockOrQueenAttackers = rockBb.getAttacksFor(fieldPosition, allPieces) & OppositeQueenAndRock;

    auto OppositeQueenAndBishop =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].bishopsMask;
    uint64_t bishopOrQueenAttackers = bishopBb.getAttacksFor(fieldPosition, allPieces) & OppositeQueenAndBishop;

    return attackingPawn | attackingKnights | attackingKings | rockOrQueenAttackers | bishopOrQueenAttackers;
}


unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    return _tzcnt_u64(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char kingPos = _tzcnt_u64(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);

    return isAttackedOn(board, c, kingPos);
}

}  // namespace CheckChecker



uint64_t getFieldsSeenByOpponent(const Board& board,
                                   NOTATION::COLOR::color playerColor,
                                   unsigned char kingField) noexcept
{
    uint64_t out{};

    auto oppositeColor = playerColor + 1;
    auto oppositeColorNum = static_cast<unsigned char>(oppositeColor);
    auto kingBitField = (1ull << kingField);

    // pawns
    out |= getAllFieldsAttackedByPawns(board, oppositeColor);

    // kings
    out |= getAllFieldsAttackedByKing(board, oppositeColor);

    // knights
    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    out |= getAllFieldsAttackedByKnights(allOppositeKnights);

    {
        auto allPieces = getAllOccupiedFields(board);
        auto allPiecesNoKing = allPieces ^ kingBitField;

        //Queens and bishops
        auto OppositeQueenAndBishop = board.piecesBitSets[oppositeColorNum].queensMask | board.piecesBitSets[oppositeColorNum].bishopsMask;
        for (; OppositeQueenAndBishop; OppositeQueenAndBishop = _blsr_u64(OppositeQueenAndBishop))
        {
            auto attackerField = _tzcnt_u64(OppositeQueenAndBishop);
            uint64_t attackersBBs = bishopBb.getAttacksFor(attackerField, allPiecesNoKing);
            out |= attackersBBs;
        }

        //Queens and rooks
        auto OppositeQueenAndRock = board.piecesBitSets[oppositeColorNum].queensMask | board.piecesBitSets[oppositeColorNum].rocksMask;
        for (; OppositeQueenAndRock; OppositeQueenAndRock = _blsr_u64(OppositeQueenAndRock))
        {
            // TODO: Double calculation is not efficient
            auto attackerField = _tzcnt_u64(OppositeQueenAndRock);
            uint64_t attackersBBs = rockBb.getAttacksFor(attackerField, allPiecesNoKing);
            out |= attackersBBs;
        }
    }

    return out;
}

PositionAnalyzeOut analyzePosition(const Board& board,
                                   NOTATION::COLOR::color playerColor,
                                   unsigned char kingField) noexcept
{
    PositionAnalyzeOut out;

    auto oppositeColor = playerColor + 1;
    auto oppositeColorNum = static_cast<unsigned char>(oppositeColor);
    auto kingBitField = (1ull << kingField);
    const auto& lookup = bitBoardLookup[kingField];

    // pawns
    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;
    out.fieldsSeenByOpponent |= getAllFieldsAttackedByPawns(allOppositePawns, oppositeColor);
    auto attackingPawn = allOppositePawns & getPawnAttacks(playerColor, kingField);

    // kings
    out.fieldsSeenByOpponent |= getAllFieldsAttackedByKing(board, oppositeColor);

    // knights
    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    out.fieldsSeenByOpponent |= getAllFieldsAttackedByKnights(allOppositeKnights);
    auto attackingKnights = allOppositeKnights & lookup.knightsMovePossibilities;

    {
        auto allPieces = getAllOccupiedFields(board);
        auto allPiecesNoKing = allPieces ^ kingBitField;

        //Queens and bishops
        auto OppositeQueenAndBishop = board.piecesBitSets[oppositeColorNum].queensMask | board.piecesBitSets[oppositeColorNum].bishopsMask;
        for (; OppositeQueenAndBishop; OppositeQueenAndBishop = _blsr_u64(OppositeQueenAndBishop))
        {
            auto attackerField = _tzcnt_u64(OppositeQueenAndBishop);
            uint64_t attackersBBs = bishopBb.getAttacksFor(attackerField, allPiecesNoKing);
            out.fieldsSeenByOpponent |= attackersBBs;
            if (attackersBBs & kingBitField)
            {
                ++out.kingAttackersNum;
                uint64_t kingDiagonals = fromToBits[kingField][attackerField];
                out.possibleBlockersMask |= kingDiagonals;
            }
        }

        //Queens and rooks
        auto OppositeQueenAndRock = board.piecesBitSets[oppositeColorNum].queensMask | board.piecesBitSets[oppositeColorNum].rocksMask;
        for (; OppositeQueenAndRock; OppositeQueenAndRock = _blsr_u64(OppositeQueenAndRock))
        {
            auto attackerField = _tzcnt_u64(OppositeQueenAndRock);
            uint64_t attackersBBs = rockBb.getAttacksFor(attackerField, allPiecesNoKing);
            out.fieldsSeenByOpponent |= attackersBBs;
            if (attackersBBs & kingBitField)
            {
                ++out.kingAttackersNum;
                uint64_t kingDiagonals = fromToBits[kingField][attackerField];
                out.possibleBlockersMask |= kingDiagonals ;
            }
        }
    }

    out.possibleBlockersMask |= attackingPawn;
    out.possibleBlockersMask |= attackingKnights;
    out.kingAttackersNum += bool(attackingPawn) + bool(attackingKnights);

    return out;
}
