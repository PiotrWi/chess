#include <gtest/gtest.h>
#include <utility>
#include <common/containers/HashMap.hpp>

TEST(HashMapShall, beCreated)
{
    containers::HashMap<int, unsigned  long, 5u> hashMap;

    auto hash = 5lu;
    auto key = 10lu;
    auto value = 3;
    ASSERT_FALSE(hashMap.get(hash, key));

    hashMap.store(hash, key, value);
    ASSERT_TRUE(hashMap.get(hash, key));
    ASSERT_EQ(*hashMap.get(hash, key), value);

    ASSERT_FALSE(hashMap.get(hash+1, key));
    ASSERT_FALSE(hashMap.get(hash, key+1));

    auto anotherValue = 4;
    auto anotherKey = 11lu;
    hashMap.store(hash, anotherKey, anotherValue);
    ASSERT_EQ(*hashMap.get(hash, key), value);
    ASSERT_EQ(*hashMap.get(hash, anotherKey), anotherValue);
    // hashMap.removeOlderThan(3);
    hashMap = std::move(containers::HashMap<int, unsigned  long, 5u>());
    std::cout << "lock" << std::endl;

}
