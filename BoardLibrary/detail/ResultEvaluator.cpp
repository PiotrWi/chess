#include "ResultEvaluator.hpp"

#include <algorithm>
#include <stdint.h>
#include <vector>

#include <detail/CheckChecker.hpp>
#include <MoveGeneratorV2/MoveGeneratorV2.hpp>
#include <publicIf/Notation.hpp>

Node::Node() noexcept {}

Node::Node(const Board& b,
		unsigned char rT,
		unsigned char noSignificant) noexcept
	: board(b)
	, repeatedTime(rT)
	, noSignificantMoves_(noSignificant)
{
}

Node TAIL = {{}, 0, 0};

namespace
{

bool are3Repeatitions(std::vector<Node>& nodes)
{
	auto compare = [](const Board& lhs, const Board& rhs) {
        return lhs.piecesBitSets[0] == rhs.piecesBitSets[0] && lhs.piecesBitSets[1] == rhs.piecesBitSets[1];
	};

	int significantCounter = nodes.back().noSignificantMoves_ - 1;
    auto boardTemplate = nodes.back().board;
    for (auto it = nodes.rbegin() + 1; significantCounter >= 0; ++it, --significantCounter)
    {
        if (compare(boardTemplate, it->board))
        {
            nodes.back().repeatedTime = it->repeatedTime + 1;
            return 2 == nodes.back().repeatedTime;
        }
    }
    return false;
}

}  // namespace

ResultEvaluator::ResultEvaluator()
	: boardsToEvaluate({TAIL})
{
}

void ResultEvaluator::storeBoard(const Board& board, const ExtendedMove& move)
{
    boardsToEvaluate.emplace_back(
        board,
        0,
        ((move.flags & (ExtendedMove::pawnMoveMask | ExtendedMove::beatingMask))
            ? 0 : boardsToEvaluate.back().noSignificantMoves_ + 1));
}

void ResultEvaluator::removeSingle()
{
    boardsToEvaluate.pop_back();
}

Result ResultEvaluator::evaluate(bool movesAvailable)
{
    if (not movesAvailable)
    {
        const auto& board = boardsToEvaluate.back().board;
        if (CheckChecker::isCheckOn(board, board.playerOnMove))
        {
            return (board.playerOnMove == NOTATION::COLOR::color::white)
                       ? Result::blackWon
                       : Result::whiteWon;
        }
        return Result::draw;
    }

    if (boardsToEvaluate.back().noSignificantMoves_ >= 150)
    {
        return Result::draw;
    }

    if (are3Repeatitions(boardsToEvaluate))
    {
        return Result::draw;
    }

    return Result::ongoing;
}

Result ResultEvaluator::evaluate()
{
    auto& board = boardsToEvaluate.back().board;
    auto validMoves = MoveGenerator::MoveGeneratorV2(board, board.playerOnMove).getValidMoveCount();

    return evaluate(validMoves);
}

bool ResultEvaluator::isDrawByRepeatitions()
{
    return are3Repeatitions(boardsToEvaluate);
}

Result ResultEvaluator::getResultNoRepeatitions(bool movesAvailable) const
{
    if (not movesAvailable)
    {
        const auto& board = boardsToEvaluate.back().board;
        if (CheckChecker::isCheckOn(board, board.playerOnMove))
        {
            return (board.playerOnMove == NOTATION::COLOR::color::white)
                   ? Result::blackWon
                   : Result::whiteWon;
        }
        return Result::draw;
    }

    if (boardsToEvaluate.back().noSignificantMoves_ >= 100)
    {
        return Result::draw;
    }

    return Result::ongoing;
}
