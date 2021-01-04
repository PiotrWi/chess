#include "zobrist.hpp"

#include <publicIf/Board.hpp>
#include <random>

namespace
{

constexpr unsigned CFieldCount = 64;
constexpr unsigned CValueCount = 256;


class Hash
{
public:
    Hash()
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        for (auto i = 0u; i < CFieldCount; ++i)
        {
            for (auto j = 0u; j < CValueCount; ++j)
            {
                hashes[i][j] = dis(gen);
            }
        }
        blackHash = dis(gen);
        for (auto i = 0u; i < CFieldCount; ++i)
        {
            validEnPassant[i] = dis(gen);
        }
    }

    auto getField()
    {
        return hashes;
    }

    auto getColorHash()
    {
        return blackHash;
    }

    auto getEnPassant()
    {
        return validEnPassant;
    }
private:
    uint64_t hashes[64][256];
    uint64_t blackHash;
    uint64_t validEnPassant[65];
};

Hash HASH;

}  // namespace

uint64_t hash(Board& board) noexcept
{
    uint64_t h = 0u;
    for (unsigned char i = 0u; i < 64; ++i)
    {
        h ^= HASH.getField()[i][board[i]];
    }

    if (board.playerOnMove == NOTATION::COLOR::color::black)
    {
        h ^= HASH.getColorHash();
    }

    h ^= HASH.getEnPassant()[board.validEnPassant+1];

    return h;
}

uint64_t switchColor(uint64_t oldHash) noexcept
{
    return oldHash ^= HASH.getColorHash();
}

uint64_t switchField(uint64_t oldHash, unsigned char fieldIndex, unsigned char oldVal, unsigned char newVal) noexcept
{
    oldHash ^= HASH.getField()[fieldIndex][oldVal];
    return oldHash ^= HASH.getField()[fieldIndex][newVal];
}

uint64_t switchEnPassant(uint64_t oldHash, unsigned char oldVal, unsigned char newVal) noexcept
{
    oldHash ^= HASH.getEnPassant()[oldVal];
    return oldHash ^= HASH.getEnPassant()[newVal];
}
