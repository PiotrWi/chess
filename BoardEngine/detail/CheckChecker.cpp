#include "CheckChecker.hpp"

#include <algorithm>
#include <array>
#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

#include <FieldLookup/FieldLookup.hpp>

namespace CheckChecker
{

constexpr uint64_t NOT_H_COL = 0x7f'7f'7f'7f'7f'7f'7f'7full;
constexpr uint64_t NOT_HG_COL = 0x3f'3f'3f'3f'3f'3f'3f'3full;
constexpr uint64_t NOT_A_COL = 0xfe'fe'fe'fe'fe'fe'fe'feull;
constexpr uint64_t NOT_AB_COL = 0xfc'fc'fc'fc'fc'fc'fc'fcull;

constexpr uint64_t onlyLeftFrom[8] =
{
        0x00'00'00'00'00'00'00'00ull,
        0x01'01'01'01'01'01'01'01ull,
        0x03'03'03'03'03'03'03'03ull,
        0x07'07'07'07'07'07'07'07ull,
        0x0f'0f'0f'0f'0f'0f'0f'0full,
        0x1f'1f'1f'1f'1f'1f'1f'1full,
        0x3f'3f'3f'3f'3f'3f'3f'3full,
        0x7f'7f'7f'7f'7f'7f'7f'7full,
};

constexpr uint64_t onlyRightFrom[8] =
{
        0xfe'fe'fe'fe'fe'fe'fe'feull,
        0xfc'fc'fc'fc'fc'fc'fc'fcull,
        0xf8'f8'f8'f8'f8'f8'f8'01ull,
        0xf0'f0'f0'f0'f0'f0'f0'f0ull,
        0xe0'e0'e0'e0'e0'e0'e0'e0ull,
        0xc0'c0'c0'c0'c0'c0'c0'c0ull,
        0x80'80'80'80'80'80'80'80ull,
        0x00'00'00'00'00'00'00'00ull,
};

struct BitBoardsConstants
{
	uint64_t OppositePawnsAttackingFieldForWhite;
	uint64_t OppositePawnsAttackingFieldForBlack;
	uint64_t knightsMovePossibilities;
	uint64_t kingMovePossibilities;
};

constexpr std::array<BitBoardsConstants, 64> createLookups()
{
	std::array<BitBoardsConstants, 64> lookups = {};
	for (unsigned int field = 0; field < 64; ++field)
	{
		uint64_t fieldBitMask = (1ull << field);
		lookups[field].OppositePawnsAttackingFieldForWhite =
			(NOT_A_COL & fieldBitMask) << 7
                	| (NOT_H_COL & fieldBitMask) << 9;
		lookups[field].OppositePawnsAttackingFieldForBlack = 
                	(NOT_A_COL & fieldBitMask) >> 9
                	| (NOT_H_COL & fieldBitMask) >> 7;
		lookups[field].knightsMovePossibilities =
			(NOT_HG_COL & fieldBitMask) << 10
            		| (NOT_HG_COL & fieldBitMask) >> 6
            		| (NOT_H_COL & fieldBitMask) << 15
            		| (NOT_H_COL & fieldBitMask) >> 17
            		| (NOT_AB_COL & fieldBitMask) << 6
            		| (NOT_AB_COL & fieldBitMask) >> 10
            		| (NOT_A_COL & fieldBitMask) << 17
            		| (NOT_A_COL & fieldBitMask) >> 15;
                lookups[field].kingMovePossibilities =
			kingBitMask << 8
            		| kingBitMask >> 8
            		| (NOT_A_COL & kingBitMask) << 7
            		| (NOT_A_COL & kingBitMask) >> 1
            		| (NOT_A_COL & kingBitMask) >> 9
            		| (NOT_H_COL & kingBitMask) << 9
            		| (NOT_H_COL & kingBitMask) << 1
            		| (NOT_H_COL & kingBitMask) >> 7
	}
	return lookups;
}

constexpr std::array<BitBoardsConstants, 64> bitBoardLookup = createLookups();

// TODO: All of these bitmast shall be precalculated once
bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);

    auto kingBitMask = (1ull << fieldPosition);
    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;

    uint64_t oppositeAttackingPawnsCandidates;
    if (playerColor == NOTATION::COLOR::color::white)
    {
        oppositeAttackingPawnsCandidates =
                (NOT_A_COL & kingBitMask) << 7
                | (NOT_H_COL & kingBitMask) << 9;
    } else
    {
        oppositeAttackingPawnsCandidates =
                (NOT_A_COL & kingBitMask) >> 9
                | (NOT_H_COL & kingBitMask) >> 7;
    }
    auto attackingPawn = oppositeAttackingPawnsCandidates & allOppositePawns;

    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    uint64_t oppositeAttackingKnightsCandidates =
            (NOT_HG_COL & kingBitMask) << 10
            | (NOT_HG_COL & kingBitMask) >> 6
            | (NOT_H_COL & kingBitMask) << 15
            | (NOT_H_COL & kingBitMask) >> 17
            | (NOT_AB_COL & kingBitMask) << 6
            | (NOT_AB_COL & kingBitMask) >> 10
            | (NOT_A_COL & kingBitMask) << 17
            | (NOT_A_COL & kingBitMask) >> 15;
    auto attackingKnights = allOppositeKnights & oppositeAttackingKnightsCandidates;

    auto oppositeKing = board.piecesBitSets[oppositeColorNum].kingsMask;
    uint64_t oppositeAttackingKingCandidates =
            kingBitMask << 8
            | kingBitMask >> 8
            | (NOT_A_COL & kingBitMask) << 7
            | (NOT_A_COL & kingBitMask) >> 1
            | (NOT_A_COL & kingBitMask) >> 9
            | (NOT_H_COL & kingBitMask) << 9
            | (NOT_H_COL & kingBitMask) << 1
            | (NOT_H_COL & kingBitMask) >> 7;
    auto attackingKings = oppositeKing & oppositeAttackingKingCandidates;

    auto allPieces = board.piecesBitSets[0].rocksMask
                     | board.piecesBitSets[0].queensMask
                     | board.piecesBitSets[0].kingsMask
                     | board.piecesBitSets[0].knightsMask
                     | board.piecesBitSets[0].pawnsMask
                     | board.piecesBitSets[0].bishopsMask
                     | board.piecesBitSets[1].rocksMask
                     | board.piecesBitSets[1].queensMask
                     | board.piecesBitSets[1].kingsMask
                     | board.piecesBitSets[1].knightsMask
                     | board.piecesBitSets[1].pawnsMask
                     | board.piecesBitSets[1].bishopsMask;

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;
    uint64_t rockOrQueenAttackers = 0;
    // NORTH
    auto northMask = kingBitMask << 8;
    northMask |= northMask << 8;
    northMask |= northMask << 16;
    northMask |= northMask << 32;
    auto northPieces = northMask & allPieces;
    if (northPieces != 0)
    {
        rockOrQueenAttackers |= (1ull << (__builtin_ffsll(northPieces) - 1)) & OppositeQueenAndRock;
    }

    // RIGHT
    uint64_t rightMask = 0ull;
    for (auto i = fieldPosition % 8 + 1; i < 8; ++i)
    {
        rightMask |= (rightMask << 1) | (kingBitMask << 1);
    }
    auto rightPieces = rightMask & allPieces;
    if (rightPieces != 0)
    {
        rockOrQueenAttackers |= (1ull << (__builtin_ffsll(rightPieces) - 1)) & OppositeQueenAndRock;
    }

    // LEFT
    uint64_t leftMask = 0ull;
    for (auto i = fieldPosition % 8 - 1; i > -1 ; --i)
    {
        leftMask |= (leftMask >> 1) | (kingBitMask >> 1);
    }
    auto leftPieces = leftMask & allPieces;
    if (leftPieces != 0)
    {
        rockOrQueenAttackers |= (1ull << (63 - __builtin_clzll(leftPieces))) & OppositeQueenAndRock;
    }

    // BOTTOM
    auto bottomMask = kingBitMask >> 8;
    bottomMask |= bottomMask >> 8;
    bottomMask |= bottomMask >> 16;
    bottomMask |= bottomMask >> 32;
    auto bottomPieces = bottomMask & allPieces;
    if (bottomPieces != 0)
    {
        rockOrQueenAttackers |= (1ull << (63 - __builtin_clzll(bottomPieces))) & OppositeQueenAndRock;
    }

    auto OppositeQueenAndBishop =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].bishopsMask;
    uint64_t bishopOrQueenAttackers = 0;

    // LEFT UP
    auto leftUpMask = kingBitMask << 7;
    leftUpMask |= leftUpMask << 7;
    leftUpMask |= leftUpMask << 14;
    leftUpMask |= leftUpMask << 28;
    leftUpMask &= onlyLeftFrom[fieldPosition % 8];
    auto leftUpPieces = leftUpMask & allPieces;
    if (leftUpPieces != 0)
    {
        bishopOrQueenAttackers |= (1ull << (__builtin_ffsll(leftUpPieces) - 1)) & OppositeQueenAndBishop;
    }

    // RIGHT UP
    auto rightUpMask = kingBitMask << 9;
    rightUpMask |= rightUpMask << 9;
    rightUpMask |= rightUpMask << 18;
    rightUpMask |= rightUpMask << 36;
    rightUpMask &= onlyRightFrom[fieldPosition % 8];
    auto rightUpPieces = rightUpMask & allPieces;
    if (rightUpPieces != 0)
    {
        bishopOrQueenAttackers |= (1ull << (__builtin_ffsll(rightUpPieces) - 1)) & OppositeQueenAndBishop;
    }

    // LEFT BOTTOM
    auto leftBottomMask = kingBitMask >> 9;
    leftBottomMask |= leftBottomMask >> 9;
    leftBottomMask |= leftBottomMask >> 18;
    leftBottomMask |= leftBottomMask >> 36;
    leftBottomMask &= onlyLeftFrom[fieldPosition % 8];
    auto leftBottomPieces = leftBottomMask & allPieces;
    if (leftBottomPieces != 0)
    {
        bishopOrQueenAttackers |= (1ull << (63 - __builtin_clzll(leftBottomPieces))) & OppositeQueenAndBishop;
    }

    // RIGHT BOTTOM
    auto rightBottomMask = kingBitMask >> 7;
    rightBottomMask |= rightBottomMask >> 7;
    rightBottomMask |= rightBottomMask >> 14;
    rightBottomMask |= rightBottomMask >> 28;
    rightBottomMask &= onlyRightFrom[fieldPosition % 8];
    auto rightBottomPieces = rightBottomMask & allPieces;
    if (rightBottomPieces != 0)
    {
        bishopOrQueenAttackers |= (1ull << (63 - __builtin_clzll(rightBottomPieces))) & OppositeQueenAndBishop;
    }
    return attackingPawn | attackingKnights | attackingKings | rockOrQueenAttackers | bishopOrQueenAttackers;
}

unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    return 63 - __builtin_clzll(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char kingPos = 63 - __builtin_clzll(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);

    return isAttackedOn(board, c, kingPos);
}

}  // namespace CheckChecker
