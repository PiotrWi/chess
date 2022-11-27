#include <gtest/gtest.h>

#include <vector>

#include <common/containers/PoolAllocator.hpp>

TEST(PoolAllocatorShall, BeConstructable)
{
    PoolAllocator<double> pa;

    for (int i = 0; i < 256; ++i)
    {
    	pa.allocate();
    }
}

TEST(PoolAllocatorShall, AllocateAndDealocate)
{
	std::vector <double*> toClear;
    PoolAllocator<double> pa;

    auto* ptr = pa.allocate();
    pa.dealocate(ptr);
}


TEST(PoolAllocatorShall, AllocateAndDealocate5Elements)
{
	std::vector <double*> toClear;
    PoolAllocator<double> pa;

    for (int i = 0; i < 300; ++i)
    {
    	auto* ptr = pa.allocate();
    	toClear.push_back(ptr);
    }

    for (auto&& elem : toClear)
    {
    	pa.dealocate(elem);
    }
    for (int i = 0; i < 300; ++i)
    {
    	pa.allocate();
    }
}