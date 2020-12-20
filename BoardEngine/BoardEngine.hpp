#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/Result.hpp>

#include <detail/ResultEvaluator.hpp>

class BoardEngine
{
public:
    BoardEngine();
    bool validateMove(const Move&);
    void applyMove(const Move&);
    Result getResult();

    Board board;
    ResultEvaluator resultEvaluator;
};