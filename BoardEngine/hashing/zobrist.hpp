#pragma once

#include <cstdint>

class Board;

namespace hash
{

uint64_t hash(Board &) noexcept;

uint64_t switchColor(uint64_t oldHash) noexcept;

uint64_t switchField(uint64_t oldHash, unsigned char fieldIndex, unsigned char oldVal, unsigned char newVal) noexcept;

uint64_t switchEnPassant(uint64_t oldHash, unsigned char oldVal, unsigned char newVal) noexcept;

}
