#pragma once

#include <string>

#include <publicIf/Board.hpp>

template <typename TKey, typename TValue>
struct JsonElem
{
	TKey key;
	TValue Value;
};

template <typename TKey, typename TValue>
JsonElem<TKey, TValue> createJsonElem(TKey&& key, TValue&& value)
{
	return JsonElem<TKey, TValue>(key, value);
}

std::string serialize(const Board& board)
{
	char field[3] = "a1";

	for (unsigned char row : {8u, 7u, 6u, 5u, 4u, 3u, 2u, 1u})
	{

	}
	std::string JSON;
	return JSON;
}
