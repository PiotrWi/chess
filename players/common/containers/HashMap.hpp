#pragma once

#include <utility>
#include <vector>

namespace containers
{

template<typename TValue, typename TKey, unsigned char THashWidth>
class HashMap
{
    struct Node
    {
        TKey key;
        TValue val;
        Node *next;

        Node (const TKey& keyIn, const TValue& valIn, Node* nextIn)
            : key(keyIn)
            , val(valIn)
            , next(nextIn)
        {}

        Node (const TKey& keyIn, TValue&& valIn, Node* nextIn)
                : key(keyIn)
                , val(std::move(valIn))
                , next(nextIn)
        {}

        ~Node()
        {
            delete next;
        }
    };

public:
    HashMap() :
            table(1 << THashWidth)
    {
    }
    ~HashMap()
    {
        for (auto* elem : table)
        {
            delete elem;
        }
    }
    const TValue* get(uint64_t hash, const TKey& key)
    {
        auto index = hash & hashMask;
        Node* node = table[index];
        while (node != nullptr)
        {
            if (node->key == key)
            {
                return &(node->val);
            }
            node = node->next;
        }
        return nullptr;
    }

    const TValue* store(uint64_t hash, const TKey& key, const TValue& val)
    {
        auto index = hash & hashMask;
        table[index] = new Node{key, val, table[index]};
        return &(table[index]->val);
    }

    const TValue* store(uint64_t hash, const TKey& key, const TValue&& val)
    {
        auto index = hash & hashMask;
        table[index] = new Node{key, std::move(val), table[index]};
        return &(table[index]->val);
    }

private:
    std::vector<Node *> table;
    static constexpr uint64_t hashMask = (1 << THashWidth) - 1;
};

}  // namespace containers
