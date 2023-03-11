#include <gtest/gtest.h>
#include <core/bitboards/FromToBits.hpp>

TEST(FromToBits, shallGenerateCorrectBitMaps)
{
    auto e5 ="e5"_field;
    {
        ASSERT_EQ(fromToBits[e5]["d1"_field], 0ull);
    }
    {
        auto expectation = "e1"_bit | "e2"_bit | "e3"_bit | "e4"_bit;
        ASSERT_EQ(fromToBits[e5]["e1"_field], expectation);
    }
}