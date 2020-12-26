#pragma once

#include <memory>

#include <publicIf/Board.hpp>
#include <publicIf/Result.hpp>

struct Node;

class ResultEvaluator
{
public:
	ResultEvaluator();
    ResultEvaluator(const ResultEvaluator&);
	~ResultEvaluator();
public:
	void storeBoard(const Board&);
    void removeSingle();

	Result evaluate() const;
    Result evaluate(bool movesAvailable) const;

    std::unique_ptr<Node> boardsToEvaluate;
};
