#pragma once

#include <optional>
#include <utility>

namespace containers
{

template<typename TValue, typename TKey, unsigned char THashWidth>
class HashTable
{
public:
    HashTable():
        table(1 << THashWidth)
    {
    }
    std::optional<std::pair<TKey, TValue>> get(uint64_t hash, const TKey& key)
    {
        auto index = hash & hashMask;
        const auto& elem = table[index];
        return (elem and elem->first == key) ? elem : std::optional<std::pair<TKey, TValue>>{};
    }

    void store(uint64_t hash, const TKey& key, const TValue& value)
    {
        auto index = hash & hashMask;
        table[index] = std::make_pair(key, value);
    }

private:
    std::vector<std::optional<std::pair<TKey, TValue>>> table;
    static constexpr uint64_t hashMask = (1 << THashWidth) - 1;
};

}
