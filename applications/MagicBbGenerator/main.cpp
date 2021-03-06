#include <detail/bitboardslookups.hpp>

#include <iostream>
#include <vector>
#include <random>
#include <cstring>

#include <thread>

std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<uint64_t> dis;

uint64_t tryForRock = 12;
uint64_t numOfSpecyficForRock[64] =
    {12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12};
MagicBitBoard<8192> bb_rock;
bool isUsed_rock[8192] = {};

uint64_t tryForBishop = 9;
uint64_t numOfSpecyficForBishop[64] =
    {6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 7, 7, 7, 7, 6, 6,
    6, 6, 7, 9, 9, 7, 6, 6,
    6, 6, 7, 9, 9, 7, 6, 6,
    6, 6, 7, 7, 7, 7, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,};
MagicBitBoard<8192> bb_bishop;
bool isUsed_bishop[8192] = {};

struct Result
{
    uint16_t numOfSpecyfic;
    uint64_t magicNum;
};

int main (int, char**)
{
    // ROCKs
    for (unsigned char i = 0u; i < 64; ++i)
    {
        auto lookup = bitBoardLookup[i];
        auto relevantBits = (horizontalRevelancyMask & (lookup.topRay | lookup.bottomRay)) | ((lookup.rightRay | lookup.leftRay) & verticalRevelancyMask);
        std::cout << "lookup["<< (unsigned)i << "].relevantBlockers = " << relevantBits << "ull;" << std::endl;
        std::cout << "lookup["<< (unsigned)i << "].relevantBitsNum = " << numOfSpecyficForRock[i] << "ull;"  << std::endl;
        std::vector<unsigned> bitIndexes = extractSetBitIndexes(relevantBits);
        auto relevantBitsCount = bitIndexes.size();

        auto bitOptionsCount = 1ul;
        for (unsigned i = 0; i < relevantBitsCount; ++i)
            bitOptionsCount *= 2;
        bool found = false;
        uint64_t magicMultiplier = 0;
        while (not found)
        {
            found = true;
            memset(isUsed_rock, 0, sizeof (isUsed_rock));
            magicMultiplier = (dis(gen) & dis(gen) & dis(gen));
            for (auto j = 0u; j < bitOptionsCount; ++j)
            {
                auto blockers = 0ull;
                for (auto k = 0u; k< relevantBitsCount; ++k)
                {
                    if ((1 << k) & j)
                    {
                        blockers |= (1ull << bitIndexes[k]);
                    }
                };
                auto attacks = evaluateLineAttacks(blockers, i);
                auto key = blockers * magicMultiplier >> (64 - numOfSpecyficForRock[i]);
                if (isUsed_rock[key] and bb_rock.attacks[key] != attacks)
                {
                    found = false;
                    break;
                }
                else
                {
                    bb_rock.attacks[key] = attacks;
                    isUsed_rock[key] = true;
                }
            }

        }
        std::cout << "lookup[" << (unsigned)i << "].magicMultiplier = " << magicMultiplier  << "ull;" << std::endl;
    }

    // BISHOPs
    for (unsigned char i = 0u; i < 64; ++i)
    {
        auto lookup = bitBoardLookup[i];
        auto relevantBits = horizontalRevelancyMask & verticalRevelancyMask & (lookup.topLeft | lookup.topRight | lookup.bottomLeft | lookup.bottomRight);
        std::cout << "lookup["<< (unsigned)i << "].relevantBlockers = " << relevantBits << "ull;"  << std::endl;
        std::cout << "lookup["<< (unsigned)i << "].relevantBitsNum = " << numOfSpecyficForBishop[i] << "ull;"  << std::endl;
        std::vector<unsigned> bitIndexes = extractSetBitIndexes(relevantBits);
        auto relevantBitsCount = bitIndexes.size();

        auto bitOptionsCount = 1ul;
        for (unsigned i = 0; i < relevantBitsCount; ++i)
            bitOptionsCount *= 2;
        bool found = false;
        uint64_t magicMultiplier = 0;
        while (not found)
        {
            found = true;
            memset(isUsed_bishop, 0, sizeof (isUsed_bishop));
            magicMultiplier = (dis(gen) & dis(gen) & dis(gen));
            for (auto j = 0u; j < bitOptionsCount; ++j)
            {
                auto blockers = 0ull;
                for (auto k = 0u; k< relevantBitsCount; ++k)
                {
                    if ((1 << k) & j)
                    {
                        blockers |= (1ull << bitIndexes[k]);
                    }
                };
                auto attacks = evaluateDiagonalAttacks(blockers, i);
                auto key = blockers * magicMultiplier >> (64 - numOfSpecyficForBishop[i]);
                if (isUsed_bishop[key] and bb_bishop.attacks[key] != attacks)
                {
                    found = false;
                    break;
                }
                else
                {
                    bb_bishop.attacks[key] = attacks;
                    isUsed_bishop[key] = true;
                }
            }

        }
        std::cout << "lookup[" << (unsigned)i << "].magicMultiplier = " << magicMultiplier << "ull;"  << std::endl;
    }
    return 0;
}
