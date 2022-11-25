#pragma once

#include <iostream>

/*
Some assumptions:
1. Not thread safe.
2. Does not perform object construction/deletion. It just allocates the memory.
3. Does not return memory until destructor is called.
*/
template <typename T>
struct PoolAllocator
{
private:
	struct AllocationPool
	{
		uint64_t allocationMask[4] = {0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff};
		AllocationPool* prev = nullptr;
		AllocationPool* next = nullptr;

		struct Node
		{
			unsigned char payload_[sizeof(T)];
			AllocationPool* poolParrent_;
		} nodes_[256];
		using ptr_type = Node*;
	};
	AllocationPool* availablePools_ = nullptr;
	AllocationPool* fullyAllocatedPools_ = nullptr;
public:
	using value_type = T;

	PoolAllocator() noexcept {};

	T* allocate()
	{
		if (availablePools_ == nullptr)
		{
			availablePools_ = new AllocationPool();
			std::cout << "Created new pool: " << availablePools_ << std::endl;
		}
		for (AllocationPool* current = availablePools_; current != nullptr; current = current->next)
		{
			for (auto i = 0; i < 4; ++i)
			{
				if (current->allocationMask[i] != 0x0)
				{
					unsigned char freeBit = __builtin_ffsll(current->allocationMask[i]) - 1;
					current->allocationMask[i] ^= (1ull <<freeBit);
					std::cout << "Put to (pool, word, bit): " << current << " " << i << " " << (int)freeBit  << std::endl;

					auto freeField = i * 64 + freeBit;
					current->nodes_[freeField].poolParrent_ = current;

					if (isFull(current))
					{
						moveTofullyAllocated(current);
					}
					return (T*)(&(current->nodes_[freeField]));
				}
			}
		}
		return nullptr;
	}
	void dealocate(T* in)
	{
		AllocationPool* toBeRemoved = (((typename AllocationPool::ptr_type)(in))->poolParrent_);
		if (isFull(toBeRemoved))
		{
			std::cout << "Moving to availablePools_: " << toBeRemoved << std::endl;
			moveToAvaiable(toBeRemoved);
		}
		unsigned index = ((typename AllocationPool::ptr_type)(in)) - toBeRemoved->nodes_;
		auto wordIndex = (index >> 6);
		auto bit = index & 0b111111;
		std::cout << "Clearing (pool, word, bit): " <<  toBeRemoved << " " << wordIndex << " " << (int)bit  << std::endl;

		toBeRemoved->allocationMask[wordIndex] ^= (1ull << bit);
	}

	void clearAll()
	{
		deleteNext(availablePools_);
		deleteNext(fullyAllocatedPools_);
	}

	~PoolAllocator()
	{
		std::cout << "Deleting" << std::endl;
		deleteNext(availablePools_);
		deleteNext(fullyAllocatedPools_);
	}
private:
	bool isFull(AllocationPool* in)
	{
		return (in->allocationMask[0] == 0x0) & (in->allocationMask[1] == 0x0)
			& (in->allocationMask[2] == 0x0) & (in->allocationMask[3] == 0x0);
	}

	void moveTofullyAllocated(AllocationPool* in)
	{
		std::cout << "Moving to fullyAllocatedPools_: " << in << std::endl;

		availablePools_ = in->next;
		if (fullyAllocatedPools_ != nullptr)
		{
			fullyAllocatedPools_->prev = in;
		}
		in->next = fullyAllocatedPools_;
		fullyAllocatedPools_ = in;
		std::cout << "Moved: " << in << std::endl;
	}

	void moveToAvaiable(AllocationPool* in)
	{
		fullyAllocatedPools_ = in->next;
		if (availablePools_ != nullptr)
		{
			availablePools_->prev = in;
		}
		in->next = availablePools_;
		availablePools_ = in;
	}

	void deleteNext(AllocationPool* in)
	{
		if (in != nullptr)
		{
			deleteNext(in->next);
			std::cout << "Deleting a pool: " << in << std::endl;
			delete in;
		}
	}

};
