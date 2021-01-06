#pragma once

#include <stdint.h>
#include <vector>

#include <publicIf/Board.hpp>
#include <publicIf/Result.hpp>

struct Node
{
    Node();
    Node(const Board&,
         uint64_t,
         unsigned char ,
         unsigned char,
         unsigned char);

    Board board;
    uint64_t pawnsHash;
    unsigned char piecesCount;
    unsigned char repeatedTime = 0u;
    unsigned char noSignificantMoves_ = 0u;
};

class ResultEvaluator
{
public:
    ResultEvaluator();
	void storeBoard(const Board&);
    void removeSingle();

	Result evaluate();
    Result evaluate(bool movesAvailable);

    std::vector<Node> boardsToEvaluate;
};
