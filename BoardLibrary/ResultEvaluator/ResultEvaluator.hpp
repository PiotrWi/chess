#pragma once

#include <stdint.h>
#include <vector>

#include "core/Board.hpp"
#include "Result.hpp"

struct Node
{
    Node() noexcept;
    Node(const Board&,
         unsigned char,
         unsigned char) noexcept;

    Board board;
    unsigned char repeatedTime = 0u;
    unsigned char noSignificantMoves_ = 0u;
};

class ResultEvaluator
{
public:
    ResultEvaluator();

    void storeBoard(const Board& board, const ExtendedMove& move);
    void removeSingle();

	Result evaluate();
    Result evaluate(bool movesAvailable);

    bool isDrawByRepeatitions();
    Result getResultNoRepeatitions(bool movesAvailable) const;
private:
    std::vector<Node> boardsToEvaluate;
};
