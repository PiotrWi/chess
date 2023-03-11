#pragma once

#include <array>
#include "BitBoardsUtils.h"
#include <cstdint>
#include <vector>
#include <immintrin.h>

struct BitBoardsConstants
{
    uint64_t knightsMovePossibilities;
    uint64_t kingMovePossibilities;
    uint64_t topRay;
    uint64_t bottomRay;
    uint64_t leftRay;
    uint64_t rightRay;
    uint64_t topLeft;
    uint64_t topRight;
    uint64_t bottomLeft;
    uint64_t bottomRight;
};

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
        0xf8'f8'f8'f8'f8'f8'f8'f8ull,
        0xf0'f0'f0'f0'f0'f0'f0'f0ull,
        0xe0'e0'e0'e0'e0'e0'e0'e0ull,
        0xc0'c0'c0'c0'c0'c0'c0'c0ull,
        0x80'80'80'80'80'80'80'80ull,
        0x00'00'00'00'00'00'00'00ull,
};

constexpr uint64_t ranks[8] =
{
        0x01'01'01'01'01'01'01'01ull, // A
        0x02'02'02'02'02'02'02'02ull, // B
        0x04'04'04'04'04'04'04'04ull, // C
        0x08'08'08'08'08'08'08'08ull, // D
        0x10'10'10'10'10'10'10'10ull, // E
        0x20'20'20'20'20'20'20'20ull, // F
        0x40'40'40'40'40'40'40'40ull, // G
        0x80'80'80'80'80'80'80'80ull, // H
};

constexpr uint64_t getknightsMovePossibilities(uint64_t fieldBitMask)
{
    return (NOT_HG_COL & fieldBitMask) << 10
        | (NOT_HG_COL & fieldBitMask) >> 6
        | (NOT_H_COL & fieldBitMask) >> 15
        | (NOT_H_COL & fieldBitMask) << 17
        | (NOT_AB_COL & fieldBitMask) << 6
        | (NOT_AB_COL & fieldBitMask) >> 10
        | (NOT_A_COL & fieldBitMask) << 15
        | (NOT_A_COL & fieldBitMask) >> 17;
}

constexpr uint64_t getKingMovePossibilities(uint64_t fieldBitMask)
{
    return fieldBitMask << 8
        | fieldBitMask >> 8
        | (NOT_A_COL & fieldBitMask) << 7
        | (NOT_A_COL & fieldBitMask) >> 1
        | (NOT_A_COL & fieldBitMask) >> 9
        | (NOT_H_COL & fieldBitMask) << 9
        | (NOT_H_COL & fieldBitMask) << 1
        | (NOT_H_COL & fieldBitMask) >> 7;
}

constexpr uint64_t getTopRay(uint64_t fieldBitMask)
{
    fieldBitMask = fieldBitMask << 8;
    fieldBitMask |= fieldBitMask<< 8;
    fieldBitMask |= fieldBitMask << 16;
    fieldBitMask |= fieldBitMask << 32;
    return fieldBitMask;
}

constexpr uint64_t getBottomRay(uint64_t fieldBitMask)
{
    fieldBitMask = fieldBitMask >> 8;
    fieldBitMask |= fieldBitMask >> 8;
    fieldBitMask |= fieldBitMask >> 16;
    fieldBitMask |= fieldBitMask >> 32;
    return fieldBitMask;
}

constexpr uint64_t getRightRay(uint64_t fieldBitMask, unsigned int field)
{
    auto rightRay = 0ull;
    for (auto i = field % 8 + 1; i < 8; ++i)
    {
        rightRay |= (rightRay << 1) | (fieldBitMask << 1);
    }
    return  rightRay;
}

constexpr uint64_t getLeftRay(uint64_t fieldBitMask, unsigned int field)
{
    auto leftRay = 0ull;
    for (int i = field % 8 - 1; i > -1 ; --i)
    {
        leftRay |= (leftRay >> 1) | (fieldBitMask >> 1);
    }
    return  leftRay;
}

constexpr uint64_t getTopLeftRay(uint64_t fieldBitMask, unsigned int field)
{
    auto topLeft = fieldBitMask << 7;
    topLeft |= topLeft << 7;
    topLeft |= topLeft << 14;
    topLeft |= topLeft << 28;
    topLeft &= onlyLeftFrom[field % 8];
    return topLeft;
}

constexpr uint64_t getTopRightRay(uint64_t fieldBitMask, unsigned int field)
{
    auto topRight = fieldBitMask << 9;
    topRight |= topRight << 9;
    topRight |= topRight << 18;
    topRight |= topRight << 36;
    topRight &= onlyRightFrom[field % 8];
    return topRight;
}

constexpr uint64_t getBottomLeftRay(uint64_t fieldBitMask, unsigned int field)
{
    auto bottomLeft = fieldBitMask >> 9;
    bottomLeft |= bottomLeft >> 9;
    bottomLeft |= bottomLeft >> 18;
    bottomLeft |= bottomLeft >> 36;
    bottomLeft &= onlyLeftFrom[field % 8];
    return bottomLeft;
}

constexpr uint64_t bottomRightRay(uint64_t fieldBitMask, unsigned int field)
{
    auto bottomRight = fieldBitMask >> 7;
    bottomRight |= bottomRight >> 7;
    bottomRight |= bottomRight >> 14;
    bottomRight |= bottomRight >> 28;
    bottomRight &= onlyRightFrom[field % 8];
    return bottomRight;
}

constexpr std::array<BitBoardsConstants, 64> createLookups()
{
    std::array<BitBoardsConstants, 64> lookups = {};
    for (unsigned int field = 0; field < 64; ++field)
    {
        uint64_t fieldBitMask = (1ull << field);

        lookups[field].knightsMovePossibilities = getknightsMovePossibilities(fieldBitMask);
        lookups[field].kingMovePossibilities = getKingMovePossibilities(fieldBitMask);

        lookups[field].topRay = getTopRay(fieldBitMask);
        lookups[field].bottomRay = getBottomRay(fieldBitMask);
        lookups[field].rightRay = getRightRay(fieldBitMask, field);
        lookups[field].leftRay  = getLeftRay(fieldBitMask, field);

        lookups[field].topLeft = getTopLeftRay(fieldBitMask, field);
        lookups[field].topRight = getTopRightRay(fieldBitMask, field);
        lookups[field].bottomLeft = getBottomLeftRay(fieldBitMask, field);
        lookups[field].bottomRight = bottomRightRay(fieldBitMask, field);
    }
    return lookups;
}

std::vector<unsigned> extractSetBitIndexes(uint64_t in);

uint64_t evaluateLineAttacks(uint64_t blockers, unsigned piecePosition);
uint64_t evaluateDiagonalAttacks(uint64_t blockers, unsigned piecePosition);

static constexpr std::array<BitBoardsConstants, 64> bitBoardLookup = createLookups();

#ifndef USE_PEXT

template<unsigned TSIZE>
struct MagicBitBoard
{
    uint64_t relevantBlockers;
    uint64_t magicMultiplier;
    uint64_t attacks[TSIZE];
};

struct BishopMagicBitBoards
{
    constexpr static unsigned relevantBitsNum = 9;
    MagicBitBoard<512> lookup[64];
public:
    BishopMagicBitBoards();
    inline uint64_t getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
    {
        auto& l = lookup[fieldNum];
        unsigned key = ((l.relevantBlockers & allpieces) * l.magicMultiplier) >> (64u - relevantBitsNum);
        return l.attacks[key];
    }
};

struct RockMagicBitBoards
{
    constexpr static unsigned relevantBitsNum = 12;
    MagicBitBoard<4096u> lookup[64];
public:
    inline uint64_t getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
    {
        auto& l = lookup[fieldNum];
        unsigned key = ((l.relevantBlockers  & allpieces) * l.magicMultiplier) >> (64u - relevantBitsNum);
        return l.attacks[key];
    }
    RockMagicBitBoards();
};

struct FlexiMagicBB
{
    uint64_t relevantBlockers;
    uint8_t relevantBitsNum;
    uint64_t magicMultiplier;
    uint64_t* attacks;
};

struct BishopFlexiMagicBB
{
    uint64_t attacks[6656];
    FlexiMagicBB lookup[64];
public:
    BishopFlexiMagicBB();
    inline uint64_t getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
    {
        auto& l = lookup[fieldNum];
        unsigned key = ((l.relevantBlockers & allpieces) * l.magicMultiplier) >> (64u - lookup[fieldNum].relevantBitsNum);
        return l.attacks[key];
    }
};

struct RockFlexiMagicBB
{
    uint64_t attacks[102400];
    FlexiMagicBB lookup[64];
public:
    inline uint64_t getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
    {
        auto& l = lookup[fieldNum];
        unsigned key = ((l.relevantBlockers  & allpieces) * l.magicMultiplier) >> (64u - lookup[fieldNum].relevantBitsNum);
        return l.attacks[key];
    }
    RockFlexiMagicBB();
};

extern BishopFlexiMagicBB bishopBb;
extern RockFlexiMagicBB rockBb;

#endif 

#ifdef USE_PEXT

struct PextForField
{
    uint64_t relevantBlockers;
    uint8_t relevantBitsNum;
    uint64_t* attacks;
};

struct BishopPextBB
{
    uint64_t attacks[5248];
    PextForField lookup[64];
public:
    BishopPextBB();
    inline uint64_t getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
    {
        auto& l = lookup[fieldNum];
        unsigned key = _pext_u64(allpieces, l.relevantBlockers);
        return l.attacks[key];
    }
};

struct RockPextBB
{
    uint64_t attacks[102400];
    PextForField lookup[64];
public:
    RockPextBB();
    inline uint64_t getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
    {
        auto& l = lookup[fieldNum];
        unsigned key = _pext_u64(allpieces, l.relevantBlockers);
        return l.attacks[key];
    }
};
extern BishopPextBB bishopBb;
extern RockPextBB rockBb;

#endif