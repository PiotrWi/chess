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
        using namespace NOTATION::COLOR;
        using namespace NOTATION::PIECES;

        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        dis(gen);
        for (auto i = 0u; i < CFieldCount; ++i)
        {
            hashes[i][WHITE | PAWN] = dis(gen);
            hashes[i][WHITE | ROCK] = dis(gen);
            hashes[i][WHITE | KNIGHT] = dis(gen);
            hashes[i][WHITE | BISHOP] = dis(gen);
            hashes[i][WHITE | QUEEN] = dis(gen);
            hashes[i][WHITE | KING] = dis(gen);
            hashes[i][BLACK | PAWN] = dis(gen);
            hashes[i][BLACK | ROCK] = dis(gen);
            hashes[i][BLACK | KNIGHT] = dis(gen);
            hashes[i][BLACK | BISHOP] = dis(gen);
            hashes[i][BLACK | QUEEN] = dis(gen);
            hashes[i][BLACK | KING] = dis(gen);
        }
        blackHash = dis(gen);
        for (auto i = 1u; i < CFieldCount; ++i)
        {
            validEnPassant[i] = dis(gen);
        }
        for (auto i = 0u; i < 16; ++i)
        {
            castlingRights[i] = dis(gen);
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

    auto getCastlingRights()
    {
        return castlingRights;
    }
private:
    uint64_t hashes[64][256] = {};
    uint64_t blackHash = {};
    uint64_t validEnPassant[65] = {};
    uint64_t castlingRights[16] = {};
};

Hash HASH;

}  // namespace

namespace hash
{

uint64_t hash(Board &board) noexcept
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

    h ^= HASH.getEnPassant()[board.validEnPassant + 1];

    h ^= HASH.getCastlingRights()[board.castlingRights];

    return h;
}

uint64_t switchColor(uint64_t oldHash) noexcept
{
    return oldHash ^ HASH.getColorHash();
}

uint64_t switchField(uint64_t oldHash, unsigned char fieldIndex, unsigned char val) noexcept
{
    return oldHash ^ HASH.getField()[fieldIndex][val];
}

uint64_t switchEnPassant(uint64_t oldHash, signed char newVal) noexcept
{
    return oldHash ^ HASH.getEnPassant()[newVal+1];
}

uint64_t switchCastlingRights(uint64_t oldHash, unsigned char val) noexcept
{
    return oldHash ^ HASH.getCastlingRights()[val];
}

}
