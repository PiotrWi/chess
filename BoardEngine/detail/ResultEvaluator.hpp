#pragma once

#include <memory>

#include <publicIf/Board.hpp>
#include <publicIf/Result.hpp>

struct Node;

class ResultEvaluator
{
public:
	ResultEvaluator();
	~ResultEvaluator();
	void storeBoard(const Board&);
	Result evaluate() const;
	void removeSingle();

	std::unique_ptr<Node> boardsToEvaluate;
};
