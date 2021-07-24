#pragma once

#include <vector>

#include <publicIf/Board.hpp>
#include <publicIf/Result.hpp>

struct Node
{
    Node();
    Node(const Board&,
         unsigned char,
         unsigned char);

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
