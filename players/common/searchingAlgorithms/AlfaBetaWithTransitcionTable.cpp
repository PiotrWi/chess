#include "AlfaBetaWithTransitctionTable.hpp"

#include "AlfaBeta.hpp"

#include <algorithm>
#include <vector>

#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>

namespace
{

int evaluateMin(BoardEngine& be,
                CachedMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta);
int evaluateMax(BoardEngine& be,
                CachedMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta);

int evaluateMin(BoardEngine& be,
                CachedMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta)
{
    auto validMoves = moveGenerator.generate(be);
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::max();
    }
    if (depth == 0)
    {
        return materialEvaluator::evaluate(be.board, color);
    }

    auto greatestValue = std::numeric_limits<int>::max();

    auto memorial = be.getMemorial();
    for (const auto & validMove : validMoves)
    {
        be.applyMove(validMove);
        auto nextBeta = evaluateMax(be, moveGenerator, depth - 1, color, alfa, beta);
        be.undoMove(memorial);

        greatestValue = std::min(greatestValue, nextBeta);
        beta = std::min(beta, nextBeta);

        if (beta <= alfa)
            break;
    }
    return greatestValue;
}

int evaluateMax(BoardEngine& be,
                CachedMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta)
{
    auto validMoves = moveGenerator.generate(be);
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::min();
    }
    if (depth == 0)
    {
        return materialEvaluator::evaluate(be.board, color);
    }

    auto memorial = be.getMemorial();
    auto greatestValue = std::numeric_limits<int>::min();
    for (const auto & validMove : validMoves)
    {
        be.applyMove(validMove);
        auto nextAlfa = evaluateMin(be, moveGenerator, depth - 1, color, alfa, beta);
        be.undoMove(memorial);

        greatestValue = std::max(greatestValue, nextAlfa);
        alfa = std::max(alfa, nextAlfa);

        if (beta <= alfa)
            break;
    }
    return greatestValue;
}

}  // namespace

namespace alfaBeta
{

Move evaluate(BoardEngine be, CachedMoveGenerator& moveGenerator, unsigned char depth)
{
    auto validMoves = moveGenerator.generate(be);
    auto greatestMove = 0u;

    int alfa = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    auto playerOnMove = be.board.playerOnMove;

    auto memorial = be.getMemorial();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, moveGenerator, depth - 1, playerOnMove, alfa, beta);
        be.undoMove(memorial);

        if (nextAlfa > alfa)
        {
            alfa = nextAlfa;
            greatestMove = i;
        }
    }
    return validMoves[greatestMove];
}

}  // namespace alfaBeta
