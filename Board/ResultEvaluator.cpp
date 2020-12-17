#include "ResultEvaluator.hpp"

#include <algorithm>
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
type TAIL = 255u;

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
	struct BoardInformations
	{
		BoardInformations();
		BoardInformations(const Board&,
			PawnHash::type,
			PiecesCount::type,
			unsigned char);

		Board board;
		PawnHash::type pawnsHash;
		PiecesCount::type piecesCount;
		unsigned char repeatedTime = 0;
	};

	std::vector<BoardInformations> boardInformations_;
	std::unique_ptr<Node> previous;

	static std::unique_ptr<Node> makeTail();
};

Node::BoardInformations::BoardInformations() {}

Node::BoardInformations::BoardInformations(const Board& b,
		PawnHash::type ph,
		PiecesCount::type pc,
		unsigned char rT)
	: board(b)
	, pawnsHash(ph)
	, piecesCount(pc)
	, repeatedTime(rT)
{
}

std::unique_ptr<Node> Node::makeTail()
{
	return std::unique_ptr<Node>(
		new Node{
			{{
				{},
				PawnHash::TAIL,
				PiecesCount::TAIL,
				0u
			}}, {}});
}

namespace
{

bool are3Repeatitions(Node& node)
{
	//TODO it definetly would advance if we lose a loop inside
	auto compare = [](const Board& lhs, const Board& rhs) {
		using namespace NOTATION;
		for (auto i = 0; i != 64; ++i)
		{
			if ((lhs[i] & COLOR_AND_PIECE_MASK) != (rhs[i] & COLOR_AND_PIECE_MASK))
				return false;
		}
		return true;
	};

	auto boardTemplate = node.boardInformations_.back().board;

	for (auto it = node.boardInformations_.begin();
			it < node.boardInformations_.end() - 1;
			++it)
	{
		if (compare(boardTemplate, it->board))
		{
			node.boardInformations_.back().repeatedTime = it->repeatedTime + 1;
			return 2 == node.boardInformations_.back().repeatedTime;
		}
	}
	return false;
}

}  // namespace

ResultEvaluator::ResultEvaluator()
	: boardsToEvaluate(Node::makeTail())
{
}

ResultEvaluator::~ResultEvaluator() {}


void ResultEvaluator::storeBoard(const Board& board)
{
	auto pawnHash = PawnHash::evaluate(board);
	auto piecesCount = PiecesCount::evaluate(board);

	if (pawnHash != boardsToEvaluate->boardInformations_.back().pawnsHash
		or piecesCount != boardsToEvaluate->boardInformations_.back().piecesCount)
	{
		auto newBoardsToEvaluate = std::unique_ptr<Node>(
			new Node{{}, std::move(boardsToEvaluate)});
		boardsToEvaluate = std::move(newBoardsToEvaluate);
	}

	boardsToEvaluate->boardInformations_.emplace_back(
		board, pawnHash, piecesCount, 0);
}

Result ResultEvaluator::evaluate() const
{
	const auto& board = boardsToEvaluate->boardInformations_.back().board;
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

	// TODO Same as upper. One is enought.
	if (MoveGenerator::MoveGenerator().generate(board, board.playerOnMove).empty())
	{
		return Result::draw;
	}

	if (boardsToEvaluate->boardInformations_.size() >= 100)
	{
		return Result::draw;
	}

	if (are3Repeatitions(*boardsToEvaluate))
	{
		return Result::draw;
	}

	return Result::ongoing;
}
