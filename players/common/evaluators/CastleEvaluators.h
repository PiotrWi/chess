#pragma once

#include <stdint.h>

struct CastlePatterns
{
    static const unsigned TSize = 10;
    struct Pattern
    {
        uint64_t pawnsPattern;
        uing64_t kingPattern;
    };
    Pattern whitePatterns[TSize];
    Pattern blackPatterns[TSize];
} castlePatterns;

int evaluateFunction(Board& be)
{
    for (decltype(CastlePatterns::TSize) i = 0; i < TSize; ++i)
    {

    }
}