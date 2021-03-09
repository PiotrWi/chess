#include <detail/bitboardslookups.hpp>

#include <iostream>
#include <vector>
#include <random>
#include <cstring>

std::vector<unsigned> extractSetBitIndexes(uint64_t in)
{
    std::vector<unsigned> out;
    for (unsigned char i = 0u; i < 64; ++i)
    {
        if ((1ull << i) & in)
        {
            out.push_back(i);
        }
    }
    return out;
}

uint64_t evaluateLine(uint64_t blockers, uint piecePosition)
{
    const auto& lookup = bitBoardLookup[piecePosition];
    uint64_t attacks = 0ull;

    {
        auto topPieces = lookup.topRay & blockers;
        if (topPieces)
        {
            auto topBlocker = __builtin_ffsll(topPieces) - 1;
            attacks |=(lookup.topRay ^ (1ull << topBlocker) ^ bitBoardLookup[topBlocker].topRay);
            attacks |= (1ull << topBlocker);
        }
        else
        {
            attacks |= lookup.topRay;
        }
    }
    {
        auto rightPieces = lookup.rightRay & blockers;
        if (rightPieces)
        {
            auto rightBlocker = __builtin_ffsll(rightPieces) - 1;
            attacks |=(lookup.rightRay ^ (1ull << rightBlocker) ^ bitBoardLookup[rightBlocker].rightRay);
            attacks |= (1ull << rightBlocker);
        }
        else
        {
            attacks |= lookup.rightRay;
        }
    }
    {
        auto leftPieces = lookup.leftRay & blockers;
        if (leftPieces)
        {
            auto leftBlocker = 63 - __builtin_clzll(leftPieces);
            attacks |=(lookup.leftRay ^ (1ull << leftBlocker) ^ bitBoardLookup[leftBlocker].leftRay);
            attacks |= (1ull << leftBlocker);
        }
        else
        {
            attacks |= lookup.leftRay;
        }
    }
    {
        auto bottomPieces = lookup.bottomRay & blockers;
        if (bottomPieces)
        {
            auto bottomBlocker = 63 - __builtin_clzll(bottomPieces);
            attacks |=(lookup.bottomRay ^ (1ull << bottomBlocker) ^ bitBoardLookup[bottomBlocker].bottomRay);
            attacks |= (1ull << bottomBlocker);
        }
        else
        {
            attacks |= lookup.bottomRay;
        }
    }
    return attacks;
}


uint64_t evaluateDiagonal(uint64_t blockers, uint piecePosition)
{
    const auto& lookup = bitBoardLookup[piecePosition];
    uint64_t attacks = 0ull;

    {
        auto topLeftPieces = lookup.topLeft & blockers;
        if (topLeftPieces)
        {
            auto topLeftBlocker = __builtin_ffsll(topLeftPieces) - 1;
            attacks |=(lookup.topLeft ^ (1ull << topLeftBlocker) ^ bitBoardLookup[topLeftBlocker].topLeft);
            attacks |= (1ull << topLeftBlocker);
        }
        else
        {
            attacks |= lookup.topLeft;
        }
    }
    {
        auto topRightPieces = lookup.topRight & blockers;
        if (topRightPieces)
        {
            auto topRightBlocker = __builtin_ffsll(topRightPieces) - 1;
            attacks |=(lookup.topRight ^ (1ull << topRightBlocker) ^ bitBoardLookup[topRightBlocker].topRight);
            attacks |= (1ull << topRightBlocker);
        }
        else
        {
            attacks |= lookup.topRight;
        }
    }
    {
        auto bottomLeftPieces = lookup.bottomLeft & blockers;
        if (bottomLeftPieces)
        {
            auto bottomLeftBlocker = 63 - __builtin_clzll(bottomLeftPieces);
            attacks |=(lookup.bottomLeft ^ (1ull << bottomLeftBlocker) ^ bitBoardLookup[bottomLeftBlocker].bottomLeft);
            attacks |= (1ull << bottomLeftBlocker);
        }
        else
        {
            attacks |= lookup.bottomLeft;
        }
    }
    {
        auto bottomRightPieces = lookup.bottomRight & blockers;
        if (bottomRightPieces)
        {
            auto bottomRightBlocker = 63 - __builtin_clzll(bottomRightPieces);
            attacks |=(lookup.bottomRight ^ (1ull << bottomRightBlocker) ^ bitBoardLookup[bottomRightBlocker].bottomRight);
            attacks |= (1ull << bottomRightBlocker);
        }
        else
        {
            attacks |= lookup.bottomRight;
        }
    }
    return attacks;
}


std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<uint64_t> dis;

uint64_t tryForRock = 12;
MagicBitBoard<8192> bb_rock;
bool isUsed_rock[8192] = {};

uint64_t tryForBishop = 10;
MagicBitBoard<8192> bb_bishop;
bool isUsed_bishop[8192] = {};

int main (int, char**)
{
    // ROCKs
    for (unsigned char i = 0u; i < 64; ++i)
    {
        auto lookup = bitBoardLookup[i];
        auto relevantBits = (horizontalRevelancyMask & (lookup.topRay | lookup.bottomRay)) | ((lookup.rightRay | lookup.leftRay) & verticalRevelancyMask);
        std::cout << "relevantBits: " << relevantBits << std::endl;
        std::vector<unsigned> bitIndexes = extractSetBitIndexes(relevantBits);
        auto relevantBitsCount = bitIndexes.size();

        auto bitOptionsCount = 1ul;
        for (unsigned i = 0; i < relevantBitsCount; ++i)
            bitOptionsCount *= 2;
        std::cout << "bitOptionsCount: " << bitOptionsCount << std::endl;
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
                auto attacks = evaluateLine(blockers, i);
                auto key = blockers * magicMultiplier >> (64 - tryForRock);
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
        std::cout << (unsigned)i << " " << magicMultiplier << std::endl;
    }

    // BISHOPs
    for (unsigned char i = 0u; i < 64; ++i)
    {
        auto lookup = bitBoardLookup[i];
        auto relevantBits = horizontalRevelancyMask & verticalRevelancyMask & (lookup.topLeft | lookup.topRight | lookup.bottomLeft | lookup.bottomRight);
        std::cout << "relevantBits: " << relevantBits << std::endl;
        std::vector<unsigned> bitIndexes = extractSetBitIndexes(relevantBits);
        auto relevantBitsCount = bitIndexes.size();

        auto bitOptionsCount = 1ul;
        for (unsigned i = 0; i < relevantBitsCount; ++i)
            bitOptionsCount *= 2;
        std::cout << "bitOptionsCount: " << bitOptionsCount << std::endl;
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
                auto attacks = evaluateDiagonal(blockers, i);
                auto key = blockers * magicMultiplier >> (64 - tryForBishop);
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
        std::cout << (unsigned)i << " " << magicMultiplier << std::endl;
    }
    return 0;
}
