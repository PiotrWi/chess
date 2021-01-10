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
	std::vector<ExtendedMove>* allMoves;
	NOTATION::COLOR::color pieceColor;
	unsigned char kingPosition;
} ctx;

bool isChecked(const ExtendedMove& move);
bool isKingCheckedAfterMove(const ExtendedMove& move);
bool allowAll(const ExtendedMove&);

bool isChecked(const ExtendedMove& move)
{
    Board boardCopy = *ctx.board;
    MoveApplier::applyMove(boardCopy, move);
    return not CheckChecker::isAttackedOn(boardCopy, ctx.board->playerOnMove, ctx.kingPosition);
}

bool isKingCheckedAfterMove(const ExtendedMove& move)
{
    Board boardCopy = *ctx.board;
    MoveApplier::applyMove(boardCopy, move);
    return not CheckChecker::isAttackedOn(boardCopy, ctx.board->playerOnMove, move.destination);
}

bool allowAll(const ExtendedMove&)
{
    return true;
}

template <bool (*isValid)(const ExtendedMove&), bool (*isValidForKing)(const ExtendedMove&), NOTATION::COLOR::color c>
class AddingStrategy
{
public:
    static void addAndPromote(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = ExtendedMove::promotionMask | ExtendedMove::pawnMoveMask;
        unsigned char pattern = ((*ctx.board)[source] & NOTATION::COLOR::COLOR_MASK);

        unsigned char pawn = NOTATION::PIECES::PAWN | pattern;

        ExtendedMove m = ExtendedMove(source, destination,
                                      MASK,
                                      NOTATION::PIECES::QUEEN | pattern,
                                      pawn, 0);
        if (not isValid(m))
        {
            return;
        }

        ctx.allMoves->emplace_back(std::move(m));
        ctx.allMoves->emplace_back(source, destination, MASK, NOTATION::PIECES::BISHOP | pattern, pawn, 0);
        ctx.allMoves->emplace_back(source, destination, MASK, NOTATION::PIECES::ROCK | pattern, pawn, 0);
        ctx.allMoves->emplace_back(source, destination, MASK, NOTATION::PIECES::KNIGHT | pattern, pawn, 0);
    }

    static void addAndPromoteWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::promotionMask
                                       | ExtendedMove::pawnMoveMask
                                       | ExtendedMove::beatingMask;

        unsigned char pattern = ((*ctx.board)[source] & NOTATION::COLOR::COLOR_MASK);

        unsigned char pawn = NOTATION::PIECES::PAWN | pattern;

        ExtendedMove m = ExtendedMove(source, destination,
                                      MASK,
                                      NOTATION::PIECES::QUEEN | pattern,
                                      pawn, targetField);
        if (not isValid(m))
        {
            return;
        }

        ctx.allMoves->emplace_back(std::move(m));
        ctx.allMoves->emplace_back(source, destination, MASK, NOTATION::PIECES::BISHOP | pattern, pawn, targetField);
        ctx.allMoves->emplace_back(source, destination, MASK, NOTATION::PIECES::ROCK | pattern, pawn, targetField);
        ctx.allMoves->emplace_back(source, destination, MASK, NOTATION::PIECES::KNIGHT | pattern, pawn, targetField);
    }

    static void addPawn(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = ExtendedMove::pawnMoveMask;
        constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, pawn, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addPawnWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::pawnMoveMask | ExtendedMove::beatingMask;
        constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, pawn, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addKnight(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char knight = NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, knight, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addKnightWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char knight = NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, knight, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addKing(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = ExtendedMove::kingMoveMask;
        constexpr unsigned char king = NOTATION::PIECES::KING | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, king, 0);
        if (not isValidForKing(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addKingWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::kingMoveMask | ExtendedMove::beatingMask;
        constexpr unsigned char knight = NOTATION::PIECES::KING | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, knight, targetField);
        if (not isValidForKing(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addRock(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char rock = NOTATION::PIECES::ROCK | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, rock, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addRockWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char rock = NOTATION::PIECES::ROCK | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, rock, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addBishop(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char bishop = NOTATION::PIECES::BISHOP | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, bishop, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addBishopWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char bishop = NOTATION::PIECES::BISHOP | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, bishop, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addQueen(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char queen = NOTATION::PIECES::QUEEN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, queen, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }

    static void addQueenWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char queen = NOTATION::PIECES::QUEEN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, queen, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.allMoves->emplace_back(std::move(m));
    }
};

template<NOTATION::COLOR::color c>
using StrategyWithAlwaysCheckChecking = AddingStrategy<isChecked, isKingCheckedAfterMove, c>;

template<NOTATION::COLOR::color c>
using StrategyWithNoChecking = AddingStrategy<allowAll, allowAll, c>;

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
            TMoveAddingStrategy::addPawn(i, i + ROW_DIFF);
        }
		if (row == FIRST_LINE and (*ctx.board)[i + 2 * ROW_DIFF] == 0)
		{
            TMoveAddingStrategy::addPawn(i, i + 2 * ROW_DIFF);
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
                TMoveAddingStrategy::addAndPromoteWithBeating(i, destination, (*ctx.board)[destination]);            }
            else
            {
                TMoveAddingStrategy::addPawnWithBeating(i, destination, (*ctx.board)[destination]);
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
                TMoveAddingStrategy::addAndPromoteWithBeating(i, destination, (*ctx.board)[destination]);            }
            else
            {
                TMoveAddingStrategy::addPawnWithBeating(i, destination, (*ctx.board)[destination]);
            }
		}
	}
}

template <NOTATION::COLOR::color c, typename TMoveAddingStrategy>
class GenerateEnPasant
{
public:
    static void proccess();
};

template<typename TMoveAddingStrategy>
class GenerateEnPasant<NOTATION::COLOR::color::white, TMoveAddingStrategy>
{
public:
static void proccess()
{
    if ((ctx.board)->validEnPassant == -1)
    {
        return;
    }
    auto enPassantCol = NotationConversions::getColumnNum((ctx.board)->validEnPassant);

    if (enPassantCol < 7u)
    {
        auto whitePawnLocCandidate = (ctx.board)->validEnPassant - NOTATION::COORDINATES::ROW_DIFF + 1;
        if ((*ctx.board)[whitePawnLocCandidate] ==
            (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
        {
            TMoveAddingStrategy::addPawnWithBeating(whitePawnLocCandidate, (ctx.board)->validEnPassant,
                                                    (*ctx.board)[(ctx.board)->validEnPassant]);
        }
    }
    if (enPassantCol > 0u)
    {
        auto whitePawnLocCandidate = (ctx.board)->validEnPassant - NOTATION::COORDINATES::ROW_DIFF - 1;
        if ((*ctx.board)[whitePawnLocCandidate] ==
            (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
        {
            TMoveAddingStrategy::addPawnWithBeating(whitePawnLocCandidate, (ctx.board)->validEnPassant,
                                                    (*ctx.board)[(ctx.board)->validEnPassant]);
        }
    }
}
};

template<typename TMoveAddingStrategy>
class GenerateEnPasant<NOTATION::COLOR::color::black, TMoveAddingStrategy>
{
public:
static void proccess()
{
    if ((ctx.board)->validEnPassant == -1)
    {
        return;
    }
    auto enPassantCol = NotationConversions::getColumnNum((ctx.board)->validEnPassant);
    if (enPassantCol < 7u)
    {
        auto whitePawnLocCandidate = (ctx.board)->validEnPassant + NOTATION::COORDINATES::ROW_DIFF + 1;
        if ((*ctx.board)[whitePawnLocCandidate] ==
            (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
        {
            TMoveAddingStrategy::addPawnWithBeating(whitePawnLocCandidate, (ctx.board)->validEnPassant,
                                                    (*ctx.board)[(ctx.board)->validEnPassant]);
        }
    }
    if (enPassantCol > 0u)
    {
        auto whitePawnLocCandidate = (ctx.board)->validEnPassant + NOTATION::COORDINATES::ROW_DIFF - 1;
        if ((*ctx.board)[whitePawnLocCandidate] ==
            (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
        {
            TMoveAddingStrategy::addPawnWithBeating(whitePawnLocCandidate, (ctx.board)->validEnPassant,
                                                    (*ctx.board)[(ctx.board)->validEnPassant]);
        }
    }
}
};

template <typename NOTATION::COLOR::color c,
        void (*TVerifyAndAdd)(unsigned char, unsigned char),
        void (*TVerifyAndAddBeating)(unsigned char, unsigned char, unsigned char),
        size_t N,
        const std::pair<unsigned char, unsigned char> TMoves[N]>
class GenerateFixedMoves
{
public:
    static void proccess(unsigned char i)
    {
        constexpr unsigned char TOpositeColor = (static_cast<unsigned char>(c) ^ NOTATION::COLOR::COLOR_MASK);
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

                if (field == 0)
                {
                    TVerifyAndAdd(i, destination);
                }
                if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
                {
                    TVerifyAndAddBeating(i, destination, field);
                }
            }
        }
    }
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateKnightMoves
{
    constexpr static std::pair<unsigned char, unsigned char> knightMoves[] = {
            {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };
public:
    static void proccess(unsigned char i)
    {
        GenerateFixedMoves<C,
            AddingStrategy<C>::addKnight,
            AddingStrategy<C>::addKnightWithBeating,
            8u, knightMoves>::proccess(i);
    }
};

template <NOTATION::COLOR::color C>
class GenerateKingMoves
{
    constexpr static const std::pair<unsigned char, unsigned char> kingMoves[] = {
        {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
public:
    static void proccess(unsigned char i)
    {
        GenerateFixedMoves<C,
                StrategyWithAlwaysCheckChecking<C>::addKing,
                StrategyWithAlwaysCheckChecking<C>::addKingWithBeating,
                8u, kingMoves>::proccess(i);
    }
};


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
        StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>::addKing(i, i-2);
	}

    if (ctx.board->castlingRights & WHITE_SHORT_BIT
        and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i+1)
        and (*ctx.board)[i+1] == 0
        and (*ctx.board)[i+2] == 0)
    {
        StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>::addKing(i, i+2);
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
        StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>::addKing(i, i-2);
    }

    if (ctx.board->castlingRights & BLACK_SHORT_BIT
        and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, i+1)
        and (*ctx.board)[i+1] == 0
        and (*ctx.board)[i+2] == 0)
    {
        StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>::addKing(i, i+2);
    }
}

template <typename NOTATION::COLOR::color color,
        void (*TVerifyAndAdd)(unsigned char, unsigned char),
        void (*TVerifyAndAddBeating)(unsigned char, unsigned char, unsigned char),
        size_t N,
        const std::pair<unsigned char, unsigned char> TMoves[N]>
class GenerateLineMoves
{
public:
    static void proccess(unsigned char i)
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
                        TVerifyAndAddBeating(i, destination, (*ctx.board)[destination]);
                    }
                    break;
                }
                TVerifyAndAdd(i, destination);
            }
        }
    };
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateRockMoves
{
    constexpr static std::pair<unsigned char, unsigned char> rockMoves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
public:
    static void proccess(unsigned char i)
    {
        GenerateLineMoves<C,
                AddingStrategy<C>::addRock,
                AddingStrategy<C>::addRockWithBeating,
                4u, rockMoves>::proccess(i);
    }
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateBishopMoves
{
    constexpr static std::pair<unsigned char, unsigned char> bishopMoves[] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
public:
    static void proccess(unsigned char i)
    {
        GenerateLineMoves<C,
                AddingStrategy<C>::addBishop,
                AddingStrategy<C>::addBishopWithBeating,
                4u, bishopMoves>::proccess(i);
    }
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateQueenMoves
{
    constexpr static std::pair<unsigned char, unsigned char> queenMoves[] =
            {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
public:
    static void proccess(unsigned char i)
    {
        GenerateLineMoves<C,
                AddingStrategy<C>::addBishop,
                AddingStrategy<C>::addBishopWithBeating,
                8u, queenMoves>::proccess(i);
    }
};

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void dispatchToGenerateStandardPawnMoves(unsigned char i);

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>,
        NOTATION::COLOR::color::white>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>,
            1u, 6u,
            NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::BLACK>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>, NOTATION::COLOR::color::black>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>,
            6u, 1u,
            -NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::WHITE>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::white>, NOTATION::COLOR::color::white>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::white>,
            1u, 6u,
            NOTATION::COORDINATES::ROW_DIFF,
            NOTATION::COLOR::BLACK>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::black>, NOTATION::COLOR::color::black>(unsigned char i)
{
    generateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::black>,
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
            dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<c>, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            GenerateRockMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
            GenerateBishopMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::QUEEN):
            GenerateQueenMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::KING):
            GenerateKingMoves<c>::proccess(i);
            return;
    }
}

template <NOTATION::COLOR::color c>
void generateWithAllMoveAllowance(unsigned char i)
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<c>, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            GenerateRockMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
            GenerateBishopMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::QUEEN):
            GenerateQueenMoves<c, StrategyWithNoChecking>::proccess(i);
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
    GenerateEnPasant<c, StrategyWithAlwaysCheckChecking<c>>::proccess();
}

template <NOTATION::COLOR::color c>
void evaluateForKing()
{
    GenerateKingMoves<c>::proccess(ctx.kingPosition);
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
    GenerateEnPasant<c, StrategyWithAlwaysCheckChecking<c>>::proccess();
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

std::vector<ExtendedMove> MoveGenerator::generate(const Board& board,
	NOTATION::COLOR::color c)
{
	std::vector<ExtendedMove> allMoves;
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

std::vector<ExtendedMove> MoveGenerator::generate(const Board& board)
{
    return generate(board, board.playerOnMove);
}

}  // namespace MoveGenerator
