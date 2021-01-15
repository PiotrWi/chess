#pragma once

#include <utility>
#include <vector>
#include <cassert>

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
        unsigned char age;

        Node (const TKey& keyIn, const TValue& valIn, Node* nextIn)
            : key(keyIn)
            , val(valIn)
            , next(nextIn)
            , age(0)
        {}

        Node (const TKey& keyIn, TValue&& valIn, Node* nextIn)
                : key(keyIn)
                , val(std::move(valIn))
                , next(nextIn)
                , age(0)
        {}

        ~Node()
        {
            delete next;
        }
    };

public:
    HashMap() :
            table(1 << THashWidth, nullptr)
    {
    }
    ~HashMap()
    {
        for (auto* elem : table)
        {
            delete elem;
        }
    }

    void increaseAge()
    {
        for (auto* elem : table)
        {
            for (auto* internalElem = elem; internalElem != nullptr; ++internalElem)
            {
                ++(internalElem->age);
            }
        }
    }

    void removeOlderThan(unsigned char maxAge)
    {
        for (auto*& elem : table)
        {
            if (elem and elem->age > maxAge)
            {
                delete elem;
            }
            if (!elem)
            {
                continue;
            }
            for (auto* prev = elem; prev->next != nullptr; prev = prev->next)
            {
                if ((prev->next->age) > maxAge)
                {
                    auto* tmp = prev->next;
                    prev->next = prev->next->next;
                    delete tmp;
                }
            }
        }
    }

    TValue* get(uint64_t hash, const TKey& key)
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

    TValue* store(uint64_t hash, const TKey& key, const TValue& val)
    {
        auto index = hash & hashMask;
        table[index] = new Node{key, val, table[index]};
        return &(table[index]->val);
    }

    TValue* store(uint64_t hash, const TKey& key, const TValue&& val)
    {
        auto index = hash & hashMask;
        table[index] = new Node{key, std::move(val), table[index]};
        return &(table[index]->val);
    }

private:
    std::vector<Node*> table;
    static constexpr uint64_t hashMask = (1 << THashWidth) - 1;
};

}  // namespace containers
