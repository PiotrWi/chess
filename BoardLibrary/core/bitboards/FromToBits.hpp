#pragma once

#include <array>
#include <stdint.h>
#include <core/NotationConversions.hpp>

namespace
{

constexpr std::array<std::array<uint64_t, 64>, 64> make()
{
    std::array<std::array<uint64_t, 64>, 64> lookups = {};

    for (auto i = 0; i < 64; ++i)
    {
        uint64_t out = 0ull;
        for (auto j = i + 8; j < 64; j += 8)
        {
            out |= (1ull << j);
            lookups[i][j] = out;
        }
        out = 0ull;
        for (auto j = i - 8; j >= 0; j -= 8)
        {
            out |= (1ull << j);
            lookups[i][j] = out;
        }
        out = 0ull;
        for (auto j = i + 1; NotationConversions::getRow(i) == NotationConversions::getRow(j); j += 1)
        {
            out |= (1ull << j);
            lookups[i][j] = out;
        }
        out = 0ull;
        for (auto j = i - 1; j >=0 && NotationConversions::getRow(i) == NotationConversions::getRow(j); j -= 1)
        {
            out |= (1ull << j);
            lookups[i][j] = out;
        }

        //diagonals
        out = 0ull;
        for (int row = NotationConversions::getRow(i) + 1, col = NotationConversions::getColumnNum(i) + 1;
            row < 8 && col < 8;
            ++row, ++col)
        {
            auto j =  NotationConversions::getFieldNum(row, col);
            out |= (1ull << j);
            lookups[i][j] = out;
        }

        out = 0ull;
        for (int row = NotationConversions::getRow(i) - 1, col = NotationConversions::getColumnNum(i) - 1;
             row >=0 && col >=0;
             --row, --col)
        {
            auto j =  NotationConversions::getFieldNum(row, col);
            out |= (1ull << j);
            lookups[i][j] = out;
        }

        out = 0ull;
        for (int row = NotationConversions::getRow(i) + 1, col = NotationConversions::getColumnNum(i) - 1;
             row <8 && col >=0;
             ++row, --col)
        {
            auto j =  NotationConversions::getFieldNum(row, col);
            out |= (1ull << j);
            lookups[i][j] = out;
        }

        out = 0ull;
        for (int row = NotationConversions::getRow(i) - 1, col = NotationConversions::getColumnNum(i) + 1;
             row >=0 && col < 8;
             --row, ++col)
        {
            auto j =  NotationConversions::getFieldNum(row, col);
            out |= (1ull << j);
            lookups[i][j] = out;
        }
    }
    return lookups;
}

}  // namespace

static constexpr std::array<std::array<uint64_t, 64>, 64> fromToBits = make();