#include "MoveGenerator.hpp"
#include <publicIf/NotationConversions.hpp>
#include <detail/CheckChecker.hpp>
#include <detail/MoveApplier.hpp>
#include <cstdint>

namespace MoveGenerator
{

namespace
{

thread_local struct MoveContext{
	const Board* board;
	std::vector<Move>* allMoves;
	NOTATION::COLOR::color pieceColor;
	unsigned char kingPosition;
} ctx;

class StrategyWithAlwaysCheckChecking
{
private:
    static bool isValid(const Move& move)
    {

        Board boardCopy = *ctx.board;
        MoveApplier::applyMove(boardCopy, move);
        return not CheckChecker::isAttackedOn(boardCopy, ctx.board->playerOnMove, ctx.kingPosition);
    }
    static bool isValidForKing(const Move& move)
    {
        Board boardCopy = *ctx.board;
        MoveApplier::applyMove(boardCopy, move);
        return not CheckChecker::isAttackedOn(boardCopy, ctx.board->playerOnMove, move.destination);
    }
public:
    static void addForUsualPiece(unsigned char source, unsigned char destination)
    {
        Move m = Move(source, destination);
        if (isValid(m))
        {
            ctx.allMoves->push_back(m);
        }
    }

    static void addForKing(unsigned char source, unsigned char destination)
    {
        Move m = Move(source, destination);
        if (isValidForKing(m))
        {
            ctx.allMoves->push_back(m);
        }
    }

    static void addAndPromote(unsigned char source, unsigned char destination)
    {
        Move m = Move(source, destination);
        if (not isValid(m))
        {
            return;
        }
        unsigned char pattern = ((*ctx.board)[source] & NOTATION::COLOR::COLOR_MASK);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::QUEEN | pattern);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::BISHOP | pattern);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::ROCK | pattern);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::KNIGHT | pattern);
    }
};

class StrategyWithNoChecking
{
public:
    static void addForUsualPiece(unsigned char source, unsigned char destination)
    {
        ctx.allMoves->emplace_back(source, destination);
    }

    static void addAndPromote(unsigned char source, unsigned char destination)
    {
        unsigned char pattern = ((*ctx.board)[source] & NOTATION::COLOR::COLOR_MASK);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::QUEEN | pattern);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::BISHOP | pattern);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::ROCK | pattern);
        ctx.allMoves->emplace_back(source, destination, true, NOTATION::PIECES::KNIGHT | pattern);
    }
};

template <typename TMoveAddingStrategy
    , unsigned char FIRST_LINE
	, unsigned char LINE_BEFORE_PROMOTION
	, signed char ROW_DIFF
	, unsigned char OPOSITE_COLOR>
void generateStandardPawnMoves(unsigned char i)
{
	if ((*ctx.board)[i + ROW_DIFF] == 0)
	{
		auto row = NotationConversions::getRow(i);
        if (row == LINE_BEFORE_PROMOTION)
        {
            TMoveAddingStrategy::addAndPromote(i, i + ROW_DIFF);
        }
        else
        {
            TMoveAddingStrategy::addForUsualPiece(i, i + ROW_DIFF);
        }
		if (row == FIRST_LINE and (*ctx.board)[i + 2 * ROW_DIFF] == 0)
		{
            TMoveAddingStrategy::addForUsualPiece(i, i + 2 * ROW_DIFF);
		}
	}
	auto col = NotationConversions::getColumnNum(i);
	if (col<7)
	{
		auto destination = i + ROW_DIFF + 1;
		if (((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
		{
            auto row = NotationConversions::getRow(i);
            if (row == LINE_BEFORE_PROMOTION)
            {
                TMoveAddingStrategy::addAndPromote(i, i + ROW_DIFF + 1);
            }
            else
            {
                TMoveAddingStrategy::addForUsualPiece(i, i + ROW_DIFF + 1);
            }
		}
	}
	if (col > 0)
	{
		auto destination = i + ROW_DIFF - 1;
		if (((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
		{
            auto row = NotationConversions::getRow(i);
            if (row == LINE_BEFORE_PROMOTION)
            {
                TMoveAddingStrategy::addAndPromote(i, i + ROW_DIFF - 1);
            }
            else
            {
                TMoveAddingStrategy::addForUsualPiece(i, i + ROW_DIFF - 1);
            }
		}
	}
}

template<typename TMoveAddingStrategy>
void generateEnPasant()
{
    if ((ctx.board)->validEnPassant == -1)
    {
        return;
    }
    auto enPassantCol = NotationConversions::getColumnNum((ctx.board)->validEnPassant);
	if (ctx.pieceColor == NOTATION::COLOR::color::white)
    {
        if (enPassantCol < 7u)
        {
            auto whitePawnLocCandidate = (ctx.board)->validEnPassant - NOTATION::COORDINATES::ROW_DIFF + 1;
            if ((*ctx.board)[whitePawnLocCandidate] ==
                (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
            {
                TMoveAddingStrategy::addForUsualPiece(whitePawnLocCandidate, (ctx.board)->validEnPassant);
            }
        }
        if (enPassantCol > 0u)
        {
            auto whitePawnLocCandidate = (ctx.board)->validEnPassant - NOTATION::COORDINATES::ROW_DIFF - 1;
            if ((*ctx.board)[whitePawnLocCandidate] ==
                (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
            {
                TMoveAddingStrategy::addForUsualPiece(whitePawnLocCandidate, (ctx.board)->validEnPassant);
            }
        }
    }
    if (ctx.pieceColor == NOTATION::COLOR::color::black)
    {
        if (enPassantCol < 7u)
        {
            auto whitePawnLocCandidate = (ctx.board)->validEnPassant + NOTATION::COORDINATES::ROW_DIFF + 1;
            if ((*ctx.board)[whitePawnLocCandidate] ==
                (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
            {
                TMoveAddingStrategy::addForUsualPiece(whitePawnLocCandidate, (ctx.board)->validEnPassant);
            }
        }
        if (enPassantCol > 0u)
        {
            auto whitePawnLocCandidate = (ctx.board)->validEnPassant + NOTATION::COORDINATES::ROW_DIFF - 1;
            if ((*ctx.board)[whitePawnLocCandidate] ==
                (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
            {
                TMoveAddingStrategy::addForUsualPiece(whitePawnLocCandidate, (ctx.board)->validEnPassant);
            }
        }
    }
}

template <void (*TVerifyAndStore)(unsigned char, unsigned char), size_t N, const std::pair<unsigned char, unsigned char> TMoves[N]>
void generateFixedMoves(unsigned char i)
{
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	for (auto* diff = TMoves; diff < TMoves + N; ++diff)
	{
		unsigned char targerRow = row + diff->first;
		unsigned char targerCol = col + diff->second;
		if (NotationConversions::isColumnInBoard(targerCol)
			&& NotationConversions::isRowInBoard(targerRow))
		{
			unsigned char destination = NotationConversions::getFieldNum(targerRow, targerCol);
			const auto& field = (*ctx.board)[destination];
			if ((field & static_cast<unsigned char>(ctx.pieceColor)) == 0)
			{
                TVerifyAndStore(i, destination);
			}
		}
	}
}

const std::pair<unsigned char, unsigned char> knightMoves[] = {
	{1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };
void (*generateKnightMovesWithCheckCheck)(unsigned char i)
    = generateFixedMoves<StrategyWithAlwaysCheckChecking::addForUsualPiece, 8, knightMoves>;
void (*generateKnightMovesWithNoCheck)(unsigned char i)
= generateFixedMoves<StrategyWithNoChecking::addForUsualPiece, 8, knightMoves>;


const std::pair<unsigned char, unsigned char> kingMoves[] = {
	{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
void (*generateNormalKingMoves)(unsigned char i) = generateFixedMoves<StrategyWithAlwaysCheckChecking::addForKing ,8, kingMoves>;

template<NOTATION::COLOR::color c>
void generateCasles(unsigned char i);

template<>
void generateCasles<NOTATION::COLOR::color::white>(unsigned char i)
{
    using namespace NOTATION::CASTLING_RIGHTS;
    if (ctx.board->castlingRights & WHITE_LONG_BIT
		and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i-1)
		and (*ctx.board)[i-1] == 0
		and (*ctx.board)[i-2] == 0
		and (*ctx.board)[i-3] == 0)
	{
        StrategyWithAlwaysCheckChecking::addForKing(i, i-2);
	}

    if (ctx.board->castlingRights & WHITE_SHORT_BIT
        and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i+1)
        and (*ctx.board)[i+1] == 0
        and (*ctx.board)[i+2] == 0)
    {
        StrategyWithAlwaysCheckChecking::addForKing(i, i+2);
    }
}

template<>
void generateCasles<NOTATION::COLOR::color::black>(unsigned char i)
{
    using namespace NOTATION::CASTLING_RIGHTS;
    if (ctx.board->castlingRights & BLACK_LONG_BIT
        and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i-1)
        and (*ctx.board)[i-1] == 0
        and (*ctx.board)[i-2] == 0
        and (*ctx.board)[i-3] == 0)
    {
        StrategyWithAlwaysCheckChecking::addForKing(i, i-2);
    }

    if (ctx.board->castlingRights & BLACK_SHORT_BIT
        and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i+1)
        and (*ctx.board)[i+1] == 0
        and (*ctx.board)[i+2] == 0)
    {
        StrategyWithAlwaysCheckChecking::addForKing(i, i+2);
    }
}

template <typename TMoveAddingStrategy, size_t N, const std::pair<unsigned char, unsigned char> TMoves[N]>
void generateLineMoves(unsigned char i)
{
	const auto row = NotationConversions::getRow(i);
	const auto col = NotationConversions::getColumnNum(i);

	for (auto* diff = TMoves; diff < TMoves + N; ++diff)
	{
		for (unsigned char r = row + diff->first, c = col + diff->second;
			r < 8u and c < 8u;
			r += diff->first, c+= diff->second)
		{
			auto destination = NotationConversions::getFieldNum(r, c);
			if ((*ctx.board)[destination] != 0)
			{
				if (static_cast<unsigned char>(ctx.pieceColor+1) ==
					((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK))
				{
                    TMoveAddingStrategy::addForUsualPiece(i, destination);
				}
				break;
			}
            TMoveAddingStrategy::addForUsualPiece(i, destination);
		}
	}
}

const std::pair<unsigned char, unsigned char> rockMoves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
void (*generateRockMoves)(unsigned char i) = generateLineMoves<StrategyWithAlwaysCheckChecking, 4, rockMoves>;
void (*generateRockMovesNoChecking)(unsigned char i) = generateLineMoves<StrategyWithNoChecking, 4, rockMoves>;


const std::pair<unsigned char, unsigned char> bishopMoves[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
void (*generateBishopMoves)(unsigned char i) = generateLineMoves<StrategyWithAlwaysCheckChecking, 4, bishopMoves>;
void (*generateBishopMovesNoChecking)(unsigned char i) = generateLineMoves<StrategyWithNoChecking, 4, bishopMoves>;

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void dispatchToGenerateStandardPawnMoves(unsigned char i);

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking, NOTATION::COLOR::color::white>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithAlwaysCheckChecking,
            1u, 6u,
            NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::BLACK>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking, NOTATION::COLOR::color::black>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithAlwaysCheckChecking,
            6u, 1u,
            -NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::WHITE>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking, NOTATION::COLOR::color::white>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithNoChecking,
            1u, 6u,
            NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::BLACK>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking, NOTATION::COLOR::color::black>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithNoChecking,
            6u, 1u,
            -NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::WHITE>(i);
}

template <NOTATION::COLOR::color c>
void dispatchToProperHandler(unsigned char i)
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            generateKnightMovesWithCheckCheck(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            generateRockMoves(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
            generateBishopMoves(i);
            return;
        case (NOTATION::PIECES::QUEEN):
            generateBishopMoves(i);
            generateRockMoves(i);
            return;
        case (NOTATION::PIECES::KING):
            generateNormalKingMoves(i);
            return;
    }
}

template <NOTATION::COLOR::color c>
void generateWithAllMoveAllowance(unsigned char i)
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            generateKnightMovesWithNoCheck(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            generateRockMovesNoChecking(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
            generateBishopMovesNoChecking(i);
            return;
        case (NOTATION::PIECES::QUEEN):
            generateBishopMovesNoChecking(i);
            generateRockMovesNoChecking(i);
            return;
    }
}

template <NOTATION::COLOR::color c>
void evaluateForCheckedPosition()
{
    for (unsigned char i = 0u; i < 64u; ++i)
    {
        dispatchToProperHandler<c>(i);
    }
    generateEnPasant<StrategyWithAlwaysCheckChecking>();
}

template <NOTATION::COLOR::color c>
void evaluateForKing()
{
    generateNormalKingMoves(ctx.kingPosition);
    generateCasles<c>(ctx.kingPosition);

}

template<NOTATION::COLOR::color c>
uint64_t evaluateTop(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto row = kingRow + 1u; row < 8u; ++row)
    {
        auto fieldNum = NotationConversions::getFieldNum(row, kingCol);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (row += 1; row < 8u; ++row)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(row, kingCol);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c>
uint64_t evaluateBottom(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto row = kingRow - 1u; row < 8u /*overflow*/; --row)
    {
        auto fieldNum = NotationConversions::getFieldNum(row, kingCol);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (row -= 1; row < 8u; --row)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(row, kingCol);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c>
uint64_t evaluateLeft(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto col = kingCol - 1u; col < 8u /*overflow*/; --col)
    {
        auto fieldNum = NotationConversions::getFieldNum(kingRow, col);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (col -= 1; col < 8u /*overflow*/; --col)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(kingRow, col);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c>
uint64_t evaluateRight(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto col = kingCol + 1u; col < 8u /*overflow*/; ++col)
    {
        auto fieldNum = NotationConversions::getFieldNum(kingRow, col);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (col += 1; col < 8u /*overflow*/; ++col)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(kingRow, col);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c>
uint64_t evaluateTopRight(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto col = kingCol + 1u, row = kingRow + 1u; row < 8u && col < 8u; ++col, ++row)
    {
        auto fieldNum = NotationConversions::getFieldNum(row, col);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (col = col + 1u, row = row + 1u; row < 8u && col < 8u; ++col, ++row)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(row, col);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c>
uint64_t evaluateTopLeft(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto col = kingCol - 1u, row = kingRow + 1u; row < 8u && col < 8u; --col, ++row)
    {
        auto fieldNum = NotationConversions::getFieldNum(row, col);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (col = col - 1u, row = row + 1u; row < 8u && col < 8u; --col, ++row)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(row, col);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}


template<NOTATION::COLOR::color c>
uint64_t evaluateBottomLeft(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto col = kingCol - 1u, row = kingRow - 1u; row < 8u && col < 8u; --col, --row)
    {
        auto fieldNum = NotationConversions::getFieldNum(row, col);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (col = col - 1u, row = row - 1u; row < 8u && col < 8u; --col, --row)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(row, col);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}


template<NOTATION::COLOR::color c>
uint64_t evaluateBottomRight(uint64_t wasChecked, unsigned char kingRow, unsigned char kingCol)
{
    for (auto col = kingCol + 1u, row = kingRow -1u; row < 8u && col < 8u; ++col, --row)
    {
        auto fieldNum = NotationConversions::getFieldNum(row, col);
        wasChecked |= (1lu << fieldNum);
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
        {
            break;
        }
        if (((*ctx.board)[fieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            bool canLeaveLine = true;
            for (col = col + 1u, row = row - 1u; row < 8u && col < 8u; ++col, --row)
            {
                auto internalFieldNum = NotationConversions::getFieldNum(row, col);
                wasChecked |= (1lu << internalFieldNum);
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c+1)
                {
                    if ((*ctx.board)[internalFieldNum] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL)
                    {
                        canLeaveLine = false;
                    }
                    break;
                }
                if (((*ctx.board)[internalFieldNum] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    generateWithAllMoveAllowance<c>(internalFieldNum);
                    break;
                }
            }
            if (canLeaveLine)
            {
                generateWithAllMoveAllowance<c>(fieldNum);
            }
            else
            {
                dispatchToProperHandler<c>(fieldNum);
            }
            break;
        }
    }
    return wasChecked;
}

template <NOTATION::COLOR::color c>
void evaluateNotCheckedPostions()
{
    uint64_t wasChecked = 0;
    wasChecked |= (1lu << ctx.kingPosition); // King moves are last to evaluate.
    auto kingRow = NotationConversions::getRow(ctx.kingPosition);
    auto kingCol = NotationConversions::getColumnNum(ctx.kingPosition);

    wasChecked = evaluateTop<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateBottom<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateLeft<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateRight<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateTopLeft<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateTopRight<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateBottomLeft<c>(wasChecked, kingRow, kingCol);
    wasChecked = evaluateBottomRight<c>(wasChecked, kingRow, kingCol);

    for (unsigned char i = 0u; i < 64u; ++i)
    {
        if (not ((1lu << i) & wasChecked) )
        {
            generateWithAllMoveAllowance<c>(i);
        }
    }
    evaluateForKing<c>();
    generateEnPasant<StrategyWithAlwaysCheckChecking>();
}

template <NOTATION::COLOR::color c>
void _generate_()
{
    auto isChecked = CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, ctx.kingPosition);
    if (isChecked)
    {
        evaluateForCheckedPosition<c>();
    }
    else
    {
        evaluateNotCheckedPostions<c>();
    }
}

}  // namespace

std::vector<Move> MoveGenerator::generate(const Board& board,
	NOTATION::COLOR::color c)
{
	std::vector<Move> allMoves;
	ctx.allMoves = &allMoves;
	ctx.board = &board;
	ctx.pieceColor = c;
    ctx.kingPosition = CheckChecker::findKing(board, c);

    if (c == NOTATION::COLOR::color::white)
    {
	    _generate_<NOTATION::COLOR::color::white>();
    }
	else
    {
        _generate_<NOTATION::COLOR::color::black>();
    }

	return allMoves;
}

std::vector<Move> MoveGenerator::generate(const Board& board)
{
    return generate(board, board.playerOnMove);
}

std::vector<MoveDecorator> MoveGenerator::generateWithAdditionalInfo(const Board&)
{
    return std::vector<MoveDecorator>{};
}

}  // namespace MoveGenerator
