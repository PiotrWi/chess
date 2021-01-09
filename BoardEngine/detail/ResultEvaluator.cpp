#include "ResultEvaluator.hpp"

#include <algorithm>
#include <stdint.h>
#include <vector>

#include <detail/CheckChecker.hpp>
#include <detail/MoveGenerator.hpp>
#include <publicIf/Notation.hpp>

namespace PawnHash
{

using type = uint64_t;

type evaluate(const Board& board)
{
	using namespace NOTATION::PIECES;

	type hash = 0;
	for (auto i = 8u; i < 56u; ++i)
	{
		hash |= (((board[i] & PIECES_MASK) == PAWN) << i);
	}
	return hash;
}

}  // namespace PawnHash

namespace PiecesCount
{

using type = unsigned char;

/*
constexpr uint64_t COLOR_64_MASK = ((uint64_t)NOTATION::COLOR::COLOR_MASK)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 8u)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 16u)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 24u)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 32u)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 40u)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 48u)
        | ((uint64_t)NOTATION::COLOR::COLOR_MASK << 56u);

type evaluate(const Board& board)
{
	using namespace NOTATION::PIECES;

    type count = 0;
	for (auto i = 0u; i < 8u; ++i)
	{
		count += __builtin_popcountll(((uint64_t*)(board.fields))[i] & COLOR_64_MASK);
	}
	return count;
}
 */

type evaluate(const Board& board)
{
	using namespace NOTATION::PIECES;

	type count = 0;
	for (auto i = 0u; i < 64u; ++i)
	{
		count += board[i] != 0;
	}
	return count;
}

}  // namespace FiguresCount

Node::Node() {}

Node::Node(const Board& b,
		PawnHash::type ph,
		PiecesCount::type pc,
		unsigned char rT,
		unsigned char noSignificant)
	: board(b)
	, pawnsHash(ph)
	, piecesCount(pc)
	, repeatedTime(rT)
	, noSignificantMoves_(noSignificant)
{
}

Node TAIL = {{}, std::numeric_limits<uint64_t>::max(), std::numeric_limits<unsigned char>::max(), 0, 0};

namespace
{

bool are3Repeatitions(std::vector<Node>& nodes)
{
	auto compare = [](const Board& lhs, const Board& rhs) {
		for (auto i = 0; i != 8; ++i)
		{
            if((*((u_int64_t*)(lhs.fields) + i)) !=
                        (*((u_int64_t*)(rhs.fields) + i)))
				return false;
		}
		return true;
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

void ResultEvaluator::storeBoard(const Board& board)
{
	auto pawnHash = PawnHash::evaluate(board);
	auto piecesCount = PiecesCount::evaluate(board);

    boardsToEvaluate.emplace_back(
            board,
            pawnHash,
            piecesCount,
            0,
            (pawnHash != boardsToEvaluate.back().pawnsHash or piecesCount != boardsToEvaluate.back().piecesCount)
                ? 0 : boardsToEvaluate.back().noSignificantMoves_ + 1);
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

    if (boardsToEvaluate.back().noSignificantMoves_ >= 100)
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
    const auto& board = boardsToEvaluate.back().board;
    return evaluate(not MoveGenerator::MoveGenerator().generate(board, board.playerOnMove).empty());
}
