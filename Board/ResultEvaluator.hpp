#pragma once

#include <memory>

#include <Board.hpp>

enum class Result
{
	ongoing,
	whiteWon,
	blackWon,
	draw
};

class Node;

class ResultEvaluator
{
public:
	ResultEvaluator();

	void storeBoard(const Board&);
	Result evaluate() const;
	void removeSingle();

	std::unique_ptr<Node> boardsToEvaluate;
};
