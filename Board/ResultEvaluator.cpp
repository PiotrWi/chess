#include "ResultEvaluator.hpp"

#include <stdint.h>
#include <utility>
#include <vector>

#include <CheckChecker.hpp>
#include <MoveGenerator.hpp>
#include <Notation.hpp>

namespace PawnHash
{

using type = uint64_t;
type TAIL = (uint64_t)(-1);

type evaluate(const Board& board)
{
	using namespace NOTATION::PIECES;

	type hash = 0;
	for (auto i = 0u; i < 64u; ++i)
	{
		hash |= (((board[i] & PIECES_MASK) == PAWN) << 1);
	}
	return hash;
}

}  // namespace PawnHash

namespace PiecesCount
{

using type = unsigned char;
type TAIL = 255;

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

struct Node
{
	std::vector<Board> boards;
	std::vector<PawnHash::type> pawnsHash;
	std::vector<PiecesCount::type> piecesCount;
	std::unique_ptr<Node> previous;

	static std::unique_ptr<Node> makeTail();
};

std::unique_ptr<Node> Node::makeTail()
{
	return std::unique_ptr<Node>(
		new Node{{},
		{PawnHash::TAIL},
		{PiecesCount::TAIL},
		{}});
}



ResultEvaluator::ResultEvaluator()
	: boardsToEvaluate(Node::makeTail())
{
}

void ResultEvaluator::storeBoard(const Board& board)
{
	auto pawnHash = PawnHash::evaluate(board);
	auto piecesCount = PiecesCount::evaluate(board);

	if (pawnHash != boardsToEvaluate->pawnsHash.back()
		or piecesCount != boardsToEvaluate->pawnsHash.back())
	{
		auto newBoardsToEvaluate = std::unique_ptr<Node>(
				new Node{{}, {}, {}, {}});
		std::swap(boardsToEvaluate, newBoardsToEvaluate->previous);
	}

	boardsToEvaluate->boards.push_back(board);
	boardsToEvaluate->pawnsHash.push_back(pawnHash);
	boardsToEvaluate->piecesCount.push_back(piecesCount);
}

Result ResultEvaluator::evaluate() const
{
	const auto& board = boardsToEvaluate->boards.back();
	if (CheckChecker::isCheckOn(board, board.playerOnMove))
	{
		// TODO One move is enought. No need to check all.
		if (MoveGenerator::MoveGenerator().generate(board, board.playerOnMove).empty())
		{
			return (board.playerOnMove == NOTATION::COLOR::color::white)
				? Result::blackWon
				: Result::whiteWon;
		}
	}

	return Result::ongoing;
}
