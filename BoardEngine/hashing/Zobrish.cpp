#include "zobrist.hpp"

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

    }
private:
    uint64_t hashes[64][256];
    uint64_t blackHash;
    uint64_t validEnPassant[64];
};

Hash HASH;

}  // namespace

uint64_t hash(Board&) noexcept
{
    return 0;
}
