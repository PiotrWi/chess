#include <gtest/gtest.h>

#include <common/containers/HashTable.hpp>

TEST(HashTableShall, beCreated)
{
    containers::HashTable<int, unsigned  long, 5u> hashTable;

    auto hash = 5lu;
    auto key = 10lu;
    auto value = 3;
    ASSERT_FALSE(hashTable.get(hash, key));

    hashTable.store(hash, key, value);
    ASSERT_TRUE(hashTable.get(hash, key));
    ASSERT_EQ(hashTable.get(hash, key)->second, value);
    ASSERT_EQ(hashTable.get(hash, key)->first, key);

    ASSERT_FALSE(hashTable.get(hash+1, key));
    ASSERT_FALSE(hashTable.get(hash+1, key+1));

    // cache miss
    auto anotherValue = 4;
    auto anotherKey = 11lu;
    hashTable.store(hash, anotherKey, anotherValue);
    ASSERT_FALSE(hashTable.get(hash, key));
    ASSERT_EQ(hashTable.get(hash, anotherKey)->second, anotherValue);
    ASSERT_EQ(hashTable.get(hash, anotherKey)->first, anotherKey);
}
