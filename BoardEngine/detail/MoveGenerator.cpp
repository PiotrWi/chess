#include "MoveGenerator.hpp"
#include <publicIf/NotationConversions.hpp>
#include <detail/CheckChecker.hpp>
#include <detail/MoveApplier.hpp>
#include <cstdint>
#include <FieldLookup/FieldLookup.hpp>

namespace MoveGenerator
{

namespace
{

thread_local struct MoveContext{
	const Board* board;
	NOTATION::COLOR::color pieceColor;
	unsigned char kingPosition;

	unsigned Nbeatings;
	ExtendedMove beatings[255];
    unsigned Npromotions;
    ExtendedMove promotions[255];
    unsigned NUsualMoves;
    ExtendedMove usualMoves[255];
    unsigned NkingMoves;
    ExtendedMove kingMoves[255];

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
        ctx.promotions[ctx.Npromotions++] = m;
        ctx.usualMoves[ctx.NUsualMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::BISHOP | pattern, pawn, 0);
        ctx.usualMoves[ctx.NUsualMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::ROCK | pattern, pawn, 0);
        ctx.usualMoves[ctx.NUsualMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::KNIGHT | pattern, pawn, 0);
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

        ctx.beatings[ctx.Nbeatings++] = m;
        ctx.usualMoves[ctx.NUsualMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::BISHOP | pattern, pawn, 0);
        ctx.usualMoves[ctx.NUsualMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::ROCK | pattern, pawn, 0);
        ctx.usualMoves[ctx.NUsualMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::KNIGHT | pattern, pawn, 0);
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
        ctx.usualMoves[ctx.NUsualMoves++] = m;
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
        ctx.beatings[ctx.Nbeatings++] = m;
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
        ctx.usualMoves[ctx.NUsualMoves++] = m;
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
        ctx.beatings[ctx.Nbeatings++] = m;
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
        ctx.kingMoves[ctx.NkingMoves++] = m;
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
        ctx.beatings[ctx.Nbeatings++] = m;
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
        ctx.usualMoves[ctx.NUsualMoves++] = m;
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
        ctx.beatings[ctx.Nbeatings++] = m;
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
        ctx.usualMoves[ctx.NUsualMoves++] = m;
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
        ctx.beatings[ctx.Nbeatings++] = m;
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
        ctx.usualMoves[ctx.NUsualMoves++] = m;
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
        ctx.beatings[ctx.Nbeatings++] = m;
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

template <NOTATION::COLOR::color C, const unsigned char i, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateKnightMoves2
{
public:
    static void proccess()
    {
        constexpr unsigned char TOpositeColor = (static_cast<unsigned char>(C) ^ NOTATION::COLOR::COLOR_MASK);

        for (auto* pos = FieldLookup<C, i>::KnightPossibleMoves;
            pos < FieldLookup<C, i>::KnightPossibleMoves + FieldLookup<C, i>::KnightPossibleMovesSize;
            ++pos)
        {
            const auto& field = (*ctx.board)[*pos];

            if (field == 0)
            {
                AddingStrategy<C>::addKnight(i, *pos);
            }
            if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
            {
                AddingStrategy<C>::addKnightWithBeating(i, *pos, field);
            }
        }
    }
};

template <NOTATION::COLOR::color C, const unsigned char i>
class GenerateKingMoves2
{
public:
    static void proccess()
    {
        constexpr unsigned char TOpositeColor = (static_cast<unsigned char>(C) ^ NOTATION::COLOR::COLOR_MASK);

        for (auto* pos = FieldLookup<C, i>::KingPossibleMoves;
             pos < FieldLookup<C, i>::KingPossibleMoves + FieldLookup<C, i>::KingPossibleMovesSize;
             ++pos)
        {
            const auto& field = (*ctx.board)[*pos];

            if (field == 0)
            {
                StrategyWithAlwaysCheckChecking<C>::addKing(i, *pos);
            }
            if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
            {
                StrategyWithAlwaysCheckChecking<C>::addKingWithBeating(i, *pos, field);
            }
        }
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
                AddingStrategy<C>::addQueen,
                AddingStrategy<C>::addQueenWithBeating,
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

template <NOTATION::COLOR::color c, const unsigned char i>
void dispatchToProperHandler()
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<c>, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves2<c, i, StrategyWithAlwaysCheckChecking>::proccess();
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
            GenerateKingMoves2<c, i>::proccess();

            return;
    }
}

template <NOTATION::COLOR::color c, const unsigned char i>
void generateWithAllMoveAllowance()
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<c>, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves2<c, i, StrategyWithNoChecking>::proccess();
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
        case (NOTATION::PIECES::KING):
            GenerateKingMoves2<c, i>::proccess();
            generateCasles<c>(ctx.kingPosition);
    }
}

template <NOTATION::COLOR::color c>
void evaluateForCheckedPosition()
{
    dispatchToProperHandler<c, 0>();
    dispatchToProperHandler<c, 1>();
    dispatchToProperHandler<c, 2>();
    dispatchToProperHandler<c, 3>();
    dispatchToProperHandler<c, 4>();
    dispatchToProperHandler<c, 5>();
    dispatchToProperHandler<c, 6>();
    dispatchToProperHandler<c, 7>();
    dispatchToProperHandler<c, 8>();
    dispatchToProperHandler<c, 9>();
    dispatchToProperHandler<c, 10>();
    dispatchToProperHandler<c, 11>();
    dispatchToProperHandler<c, 12>();
    dispatchToProperHandler<c, 13>();
    dispatchToProperHandler<c, 14>();
    dispatchToProperHandler<c, 15>();
    dispatchToProperHandler<c, 16>();
    dispatchToProperHandler<c, 17>();
    dispatchToProperHandler<c, 18>();
    dispatchToProperHandler<c, 19>();
    dispatchToProperHandler<c, 20>();
    dispatchToProperHandler<c, 21>();
    dispatchToProperHandler<c, 22>();
    dispatchToProperHandler<c, 23>();
    dispatchToProperHandler<c, 24>();
    dispatchToProperHandler<c, 25>();
    dispatchToProperHandler<c, 26>();
    dispatchToProperHandler<c, 27>();
    dispatchToProperHandler<c, 28>();
    dispatchToProperHandler<c, 29>();
    dispatchToProperHandler<c, 30>();
    dispatchToProperHandler<c, 31>();
    dispatchToProperHandler<c, 32>();
    dispatchToProperHandler<c, 33>();
    dispatchToProperHandler<c, 34>();
    dispatchToProperHandler<c, 35>();
    dispatchToProperHandler<c, 36>();
    dispatchToProperHandler<c, 37>();
    dispatchToProperHandler<c, 38>();
    dispatchToProperHandler<c, 39>();
    dispatchToProperHandler<c, 40>();
    dispatchToProperHandler<c, 41>();
    dispatchToProperHandler<c, 42>();
    dispatchToProperHandler<c, 43>();
    dispatchToProperHandler<c, 44>();
    dispatchToProperHandler<c, 45>();
    dispatchToProperHandler<c, 46>();
    dispatchToProperHandler<c, 47>();
    dispatchToProperHandler<c, 48>();
    dispatchToProperHandler<c, 49>();
    dispatchToProperHandler<c, 50>();
    dispatchToProperHandler<c, 51>();
    dispatchToProperHandler<c, 52>();
    dispatchToProperHandler<c, 53>();
    dispatchToProperHandler<c, 54>();
    dispatchToProperHandler<c, 55>();
    dispatchToProperHandler<c, 56>();
    dispatchToProperHandler<c, 57>();
    dispatchToProperHandler<c, 58>();
    dispatchToProperHandler<c, 59>();
    dispatchToProperHandler<c, 60>();
    dispatchToProperHandler<c, 61>();
    dispatchToProperHandler<c, 62>();
    dispatchToProperHandler<c, 63>();

    GenerateEnPasant<c, StrategyWithAlwaysCheckChecking<c>>::proccess();
}

template<NOTATION::COLOR::color c, unsigned char i, const unsigned char TSize, const unsigned char* begin>
uint64_t evaluateInSingleDiagonal(uint64_t wasChecked)
{
    auto piecePos = 0;
    constexpr auto TOppositeColor = c+1;

    for (const unsigned char* pos = begin;
        pos < begin + TSize;
        ++pos)
    {
        if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
        {
            break;
        }
        if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            piecePos = *pos;
            for (++pos;
                 pos < begin + TSize;
                 ++pos)
            {
                if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
                {
                    if ((*ctx.board)[*pos] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL)
                    {
                        return wasChecked | (1lu << piecePos);
                    }
                }
                if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    return wasChecked;
                }
            }
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c, unsigned char i, const unsigned char TSize, const unsigned char* begin>
uint64_t evaluateInSingleLine(uint64_t wasChecked)
{
    auto piecePos = 0;
    constexpr auto TOppositeColor = c + 1;
    for (const unsigned char* pos = begin;
         pos < begin + TSize;
         ++pos)
    {
        if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
        {
            break;
        }
        if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == c)
        {
            piecePos = *pos;

            for (++pos;
                 pos < begin + TSize;
                 ++pos)
            {
                if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
                {
                    if ((*ctx.board)[*pos] & NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES)
                    {
                        return wasChecked | (1lu << piecePos);
                    }
                }
                if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    return wasChecked;
                }
            }
        }
    }
    return wasChecked;
}

template <NOTATION::COLOR::color c, unsigned char TKingPos>
uint64_t findPinned()
{
    uint64_t pinned = 0;

    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::TopPossibleMovesSize,
            FieldLookup<c, TKingPos>::TopPossibleMoves>(pinned);
    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::BottomPossibleMovesSize,
            FieldLookup<c, TKingPos>::BottomPossibleMoves>(pinned);
    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::LeftPossibleMovesSize,
            FieldLookup<c, TKingPos>::LeftPossibleMoves>(pinned);
    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::RightPossibleMovesSize,
            FieldLookup<c, TKingPos>::RightPossibleMoves>(pinned);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::TopLeftPossibleMovesSize,
            FieldLookup<c, TKingPos>::TopLeftPossibleMoves>(pinned);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::TopRightPossibleMovesSize,
            FieldLookup<c, TKingPos>::TopRightPossibleMoves>(pinned);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::BottomLeftPossibleMovesSize,
            FieldLookup<c, TKingPos>::BottomLeftPossibleMoves>(pinned);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::BottomRightPossibleMovesSize,
            FieldLookup<c, TKingPos>::BottomRightPossibleMoves>(pinned);

    return pinned;
}

template <NOTATION::COLOR::color c>
void evaluateNotCheckedPostions(uint64_t pinnedMask)
{
    if (not ((1lu << 0) & pinnedMask)) generateWithAllMoveAllowance<c, 0>(); else dispatchToProperHandler<c, 0>();
    if (not ((1lu << 1) & pinnedMask)) generateWithAllMoveAllowance<c, 1>(); else dispatchToProperHandler<c, 1>();
    if (not ((1lu << 2) & pinnedMask)) generateWithAllMoveAllowance<c, 2>(); else dispatchToProperHandler<c, 2>();
    if (not ((1lu << 3) & pinnedMask)) generateWithAllMoveAllowance<c, 3>(); else dispatchToProperHandler<c, 3>();
    if (not ((1lu << 4) & pinnedMask)) generateWithAllMoveAllowance<c, 4>(); else dispatchToProperHandler<c, 4>();
    if (not ((1lu << 5) & pinnedMask)) generateWithAllMoveAllowance<c, 5>(); else dispatchToProperHandler<c, 5>();
    if (not ((1lu << 6) & pinnedMask)) generateWithAllMoveAllowance<c, 6>(); else dispatchToProperHandler<c, 6>();
    if (not ((1lu << 7) & pinnedMask)) generateWithAllMoveAllowance<c, 7>(); else dispatchToProperHandler<c, 7>();
    if (not ((1lu << 8) & pinnedMask)) generateWithAllMoveAllowance<c, 8>(); else dispatchToProperHandler<c, 8>();
    if (not ((1lu << 9) & pinnedMask)) generateWithAllMoveAllowance<c, 9>(); else dispatchToProperHandler<c, 9>();
    if (not ((1lu << 10) & pinnedMask)) generateWithAllMoveAllowance<c, 10>(); else dispatchToProperHandler<c, 10>();
    if (not ((1lu << 11) & pinnedMask)) generateWithAllMoveAllowance<c, 11>(); else dispatchToProperHandler<c, 11>();
    if (not ((1lu << 12) & pinnedMask)) generateWithAllMoveAllowance<c, 12>(); else dispatchToProperHandler<c, 12>();
    if (not ((1lu << 13) & pinnedMask)) generateWithAllMoveAllowance<c, 13>(); else dispatchToProperHandler<c, 13>();
    if (not ((1lu << 14) & pinnedMask)) generateWithAllMoveAllowance<c, 14>(); else dispatchToProperHandler<c, 14>();
    if (not ((1lu << 15) & pinnedMask)) generateWithAllMoveAllowance<c, 15>(); else dispatchToProperHandler<c, 15>();
    if (not ((1lu << 16) & pinnedMask)) generateWithAllMoveAllowance<c, 16>(); else dispatchToProperHandler<c, 16>();
    if (not ((1lu << 17) & pinnedMask)) generateWithAllMoveAllowance<c, 17>(); else dispatchToProperHandler<c, 17>();
    if (not ((1lu << 18) & pinnedMask)) generateWithAllMoveAllowance<c, 18>(); else dispatchToProperHandler<c, 18>();
    if (not ((1lu << 19) & pinnedMask)) generateWithAllMoveAllowance<c, 19>(); else dispatchToProperHandler<c, 19>();
    if (not ((1lu << 20) & pinnedMask)) generateWithAllMoveAllowance<c, 20>(); else dispatchToProperHandler<c, 20>();
    if (not ((1lu << 21) & pinnedMask)) generateWithAllMoveAllowance<c, 21>(); else dispatchToProperHandler<c, 21>();
    if (not ((1lu << 22) & pinnedMask)) generateWithAllMoveAllowance<c, 22>(); else dispatchToProperHandler<c, 22>();
    if (not ((1lu << 23) & pinnedMask)) generateWithAllMoveAllowance<c, 23>(); else dispatchToProperHandler<c, 23>();
    if (not ((1lu << 24) & pinnedMask)) generateWithAllMoveAllowance<c, 24>(); else dispatchToProperHandler<c, 24>();
    if (not ((1lu << 25) & pinnedMask)) generateWithAllMoveAllowance<c, 25>(); else dispatchToProperHandler<c, 25>();
    if (not ((1lu << 26) & pinnedMask)) generateWithAllMoveAllowance<c, 26>(); else dispatchToProperHandler<c, 26>();
    if (not ((1lu << 27) & pinnedMask)) generateWithAllMoveAllowance<c, 27>(); else dispatchToProperHandler<c, 27>();
    if (not ((1lu << 28) & pinnedMask)) generateWithAllMoveAllowance<c, 28>(); else dispatchToProperHandler<c, 28>();
    if (not ((1lu << 29) & pinnedMask)) generateWithAllMoveAllowance<c, 29>(); else dispatchToProperHandler<c, 29>();
    if (not ((1lu << 30) & pinnedMask)) generateWithAllMoveAllowance<c, 30>(); else dispatchToProperHandler<c, 30>();
    if (not ((1lu << 31) & pinnedMask)) generateWithAllMoveAllowance<c, 31>(); else dispatchToProperHandler<c, 31>();
    if (not ((1lu << 32) & pinnedMask)) generateWithAllMoveAllowance<c, 32>(); else dispatchToProperHandler<c, 32>();
    if (not ((1lu << 33) & pinnedMask)) generateWithAllMoveAllowance<c, 33>(); else dispatchToProperHandler<c, 33>();
    if (not ((1lu << 34) & pinnedMask)) generateWithAllMoveAllowance<c, 34>(); else dispatchToProperHandler<c, 34>();
    if (not ((1lu << 35) & pinnedMask)) generateWithAllMoveAllowance<c, 35>(); else dispatchToProperHandler<c, 35>();
    if (not ((1lu << 36) & pinnedMask)) generateWithAllMoveAllowance<c, 36>(); else dispatchToProperHandler<c, 36>();
    if (not ((1lu << 37) & pinnedMask)) generateWithAllMoveAllowance<c, 37>(); else dispatchToProperHandler<c, 37>();
    if (not ((1lu << 38) & pinnedMask)) generateWithAllMoveAllowance<c, 38>(); else dispatchToProperHandler<c, 38>();
    if (not ((1lu << 39) & pinnedMask)) generateWithAllMoveAllowance<c, 39>(); else dispatchToProperHandler<c, 39>();
    if (not ((1lu << 40) & pinnedMask)) generateWithAllMoveAllowance<c, 40>(); else dispatchToProperHandler<c, 40>();
    if (not ((1lu << 41) & pinnedMask)) generateWithAllMoveAllowance<c, 41>(); else dispatchToProperHandler<c, 41>();
    if (not ((1lu << 42) & pinnedMask)) generateWithAllMoveAllowance<c, 42>(); else dispatchToProperHandler<c, 42>();
    if (not ((1lu << 43) & pinnedMask)) generateWithAllMoveAllowance<c, 43>(); else dispatchToProperHandler<c, 43>();
    if (not ((1lu << 44) & pinnedMask)) generateWithAllMoveAllowance<c, 44>(); else dispatchToProperHandler<c, 44>();
    if (not ((1lu << 45) & pinnedMask)) generateWithAllMoveAllowance<c, 45>(); else dispatchToProperHandler<c, 45>();
    if (not ((1lu << 46) & pinnedMask)) generateWithAllMoveAllowance<c, 46>(); else dispatchToProperHandler<c, 46>();
    if (not ((1lu << 47) & pinnedMask)) generateWithAllMoveAllowance<c, 47>(); else dispatchToProperHandler<c, 47>();
    if (not ((1lu << 48) & pinnedMask)) generateWithAllMoveAllowance<c, 48>(); else dispatchToProperHandler<c, 48>();
    if (not ((1lu << 49) & pinnedMask)) generateWithAllMoveAllowance<c, 49>(); else dispatchToProperHandler<c, 49>();
    if (not ((1lu << 50) & pinnedMask)) generateWithAllMoveAllowance<c, 50>(); else dispatchToProperHandler<c, 50>();
    if (not ((1lu << 51) & pinnedMask)) generateWithAllMoveAllowance<c, 51>(); else dispatchToProperHandler<c, 51>();
    if (not ((1lu << 52) & pinnedMask)) generateWithAllMoveAllowance<c, 52>(); else dispatchToProperHandler<c, 52>();
    if (not ((1lu << 53) & pinnedMask)) generateWithAllMoveAllowance<c, 53>(); else dispatchToProperHandler<c, 53>();
    if (not ((1lu << 54) & pinnedMask)) generateWithAllMoveAllowance<c, 54>(); else dispatchToProperHandler<c, 54>();
    if (not ((1lu << 55) & pinnedMask)) generateWithAllMoveAllowance<c, 55>(); else dispatchToProperHandler<c, 55>();
    if (not ((1lu << 56) & pinnedMask)) generateWithAllMoveAllowance<c, 56>(); else dispatchToProperHandler<c, 56>();
    if (not ((1lu << 57) & pinnedMask)) generateWithAllMoveAllowance<c, 57>(); else dispatchToProperHandler<c, 57>();
    if (not ((1lu << 58) & pinnedMask)) generateWithAllMoveAllowance<c, 58>(); else dispatchToProperHandler<c, 58>();
    if (not ((1lu << 59) & pinnedMask)) generateWithAllMoveAllowance<c, 59>(); else dispatchToProperHandler<c, 59>();
    if (not ((1lu << 60) & pinnedMask)) generateWithAllMoveAllowance<c, 60>(); else dispatchToProperHandler<c, 60>();
    if (not ((1lu << 61) & pinnedMask)) generateWithAllMoveAllowance<c, 61>(); else dispatchToProperHandler<c, 61>();
    if (not ((1lu << 62) & pinnedMask)) generateWithAllMoveAllowance<c, 62>(); else dispatchToProperHandler<c, 62>();
    if (not ((1lu << 63) & pinnedMask)) generateWithAllMoveAllowance<c, 63>(); else dispatchToProperHandler<c, 63>();

    GenerateEnPasant<c, StrategyWithAlwaysCheckChecking<c>>::proccess();
}

}  // namespace

static uint64_t (*whiteLookup[])()  = {
        findPinned<NOTATION::COLOR::color::white, 0>,
        findPinned<NOTATION::COLOR::color::white, 1>,
        findPinned<NOTATION::COLOR::color::white, 2>,
        findPinned<NOTATION::COLOR::color::white, 3>,
        findPinned<NOTATION::COLOR::color::white, 4>,
        findPinned<NOTATION::COLOR::color::white, 5>,
        findPinned<NOTATION::COLOR::color::white, 6>,
        findPinned<NOTATION::COLOR::color::white, 7>,
        findPinned<NOTATION::COLOR::color::white, 8>,
        findPinned<NOTATION::COLOR::color::white, 9>,
        findPinned<NOTATION::COLOR::color::white, 10>,
        findPinned<NOTATION::COLOR::color::white, 11>,
        findPinned<NOTATION::COLOR::color::white, 12>,
        findPinned<NOTATION::COLOR::color::white, 13>,
        findPinned<NOTATION::COLOR::color::white, 14>,
        findPinned<NOTATION::COLOR::color::white, 15>,
        findPinned<NOTATION::COLOR::color::white, 16>,
        findPinned<NOTATION::COLOR::color::white, 17>,
        findPinned<NOTATION::COLOR::color::white, 18>,
        findPinned<NOTATION::COLOR::color::white, 19>,
        findPinned<NOTATION::COLOR::color::white, 20>,
        findPinned<NOTATION::COLOR::color::white, 21>,
        findPinned<NOTATION::COLOR::color::white, 22>,
        findPinned<NOTATION::COLOR::color::white, 23>,
        findPinned<NOTATION::COLOR::color::white, 24>,
        findPinned<NOTATION::COLOR::color::white, 25>,
        findPinned<NOTATION::COLOR::color::white, 26>,
        findPinned<NOTATION::COLOR::color::white, 27>,
        findPinned<NOTATION::COLOR::color::white, 28>,
        findPinned<NOTATION::COLOR::color::white, 29>,
        findPinned<NOTATION::COLOR::color::white, 30>,
        findPinned<NOTATION::COLOR::color::white, 31>,
        findPinned<NOTATION::COLOR::color::white, 32>,
        findPinned<NOTATION::COLOR::color::white, 33>,
        findPinned<NOTATION::COLOR::color::white, 34>,
        findPinned<NOTATION::COLOR::color::white, 35>,
        findPinned<NOTATION::COLOR::color::white, 36>,
        findPinned<NOTATION::COLOR::color::white, 37>,
        findPinned<NOTATION::COLOR::color::white, 38>,
        findPinned<NOTATION::COLOR::color::white, 39>,
        findPinned<NOTATION::COLOR::color::white, 40>,
        findPinned<NOTATION::COLOR::color::white, 41>,
        findPinned<NOTATION::COLOR::color::white, 42>,
        findPinned<NOTATION::COLOR::color::white, 43>,
        findPinned<NOTATION::COLOR::color::white, 44>,
        findPinned<NOTATION::COLOR::color::white, 45>,
        findPinned<NOTATION::COLOR::color::white, 46>,
        findPinned<NOTATION::COLOR::color::white, 47>,
        findPinned<NOTATION::COLOR::color::white, 48>,
        findPinned<NOTATION::COLOR::color::white, 49>,
        findPinned<NOTATION::COLOR::color::white, 50>,
        findPinned<NOTATION::COLOR::color::white, 51>,
        findPinned<NOTATION::COLOR::color::white, 52>,
        findPinned<NOTATION::COLOR::color::white, 53>,
        findPinned<NOTATION::COLOR::color::white, 54>,
        findPinned<NOTATION::COLOR::color::white, 55>,
        findPinned<NOTATION::COLOR::color::white, 56>,
        findPinned<NOTATION::COLOR::color::white, 57>,
        findPinned<NOTATION::COLOR::color::white, 58>,
        findPinned<NOTATION::COLOR::color::white, 59>,
        findPinned<NOTATION::COLOR::color::white, 60>,
        findPinned<NOTATION::COLOR::color::white, 61>,
        findPinned<NOTATION::COLOR::color::white, 62>,
        findPinned<NOTATION::COLOR::color::white, 63>,
};


static uint64_t(*blackLookup[])()  = {
        findPinned<NOTATION::COLOR::color::black, 0>,
        findPinned<NOTATION::COLOR::color::black, 1>,
        findPinned<NOTATION::COLOR::color::black, 2>,
        findPinned<NOTATION::COLOR::color::black, 3>,
        findPinned<NOTATION::COLOR::color::black, 4>,
        findPinned<NOTATION::COLOR::color::black, 5>,
        findPinned<NOTATION::COLOR::color::black, 6>,
        findPinned<NOTATION::COLOR::color::black, 7>,
        findPinned<NOTATION::COLOR::color::black, 8>,
        findPinned<NOTATION::COLOR::color::black, 9>,
        findPinned<NOTATION::COLOR::color::black, 10>,
        findPinned<NOTATION::COLOR::color::black, 11>,
        findPinned<NOTATION::COLOR::color::black, 12>,
        findPinned<NOTATION::COLOR::color::black, 13>,
        findPinned<NOTATION::COLOR::color::black, 14>,
        findPinned<NOTATION::COLOR::color::black, 15>,
        findPinned<NOTATION::COLOR::color::black, 16>,
        findPinned<NOTATION::COLOR::color::black, 17>,
        findPinned<NOTATION::COLOR::color::black, 18>,
        findPinned<NOTATION::COLOR::color::black, 19>,
        findPinned<NOTATION::COLOR::color::black, 20>,
        findPinned<NOTATION::COLOR::color::black, 21>,
        findPinned<NOTATION::COLOR::color::black, 22>,
        findPinned<NOTATION::COLOR::color::black, 23>,
        findPinned<NOTATION::COLOR::color::black, 24>,
        findPinned<NOTATION::COLOR::color::black, 25>,
        findPinned<NOTATION::COLOR::color::black, 26>,
        findPinned<NOTATION::COLOR::color::black, 27>,
        findPinned<NOTATION::COLOR::color::black, 28>,
        findPinned<NOTATION::COLOR::color::black, 29>,
        findPinned<NOTATION::COLOR::color::black, 30>,
        findPinned<NOTATION::COLOR::color::black, 31>,
        findPinned<NOTATION::COLOR::color::black, 32>,
        findPinned<NOTATION::COLOR::color::black, 33>,
        findPinned<NOTATION::COLOR::color::black, 34>,
        findPinned<NOTATION::COLOR::color::black, 35>,
        findPinned<NOTATION::COLOR::color::black, 36>,
        findPinned<NOTATION::COLOR::color::black, 37>,
        findPinned<NOTATION::COLOR::color::black, 38>,
        findPinned<NOTATION::COLOR::color::black, 39>,
        findPinned<NOTATION::COLOR::color::black, 40>,
        findPinned<NOTATION::COLOR::color::black, 41>,
        findPinned<NOTATION::COLOR::color::black, 42>,
        findPinned<NOTATION::COLOR::color::black, 43>,
        findPinned<NOTATION::COLOR::color::black, 44>,
        findPinned<NOTATION::COLOR::color::black, 45>,
        findPinned<NOTATION::COLOR::color::black, 46>,
        findPinned<NOTATION::COLOR::color::black, 47>,
        findPinned<NOTATION::COLOR::color::black, 48>,
        findPinned<NOTATION::COLOR::color::black, 49>,
        findPinned<NOTATION::COLOR::color::black, 50>,
        findPinned<NOTATION::COLOR::color::black, 51>,
        findPinned<NOTATION::COLOR::color::black, 52>,
        findPinned<NOTATION::COLOR::color::black, 53>,
        findPinned<NOTATION::COLOR::color::black, 54>,
        findPinned<NOTATION::COLOR::color::black, 55>,
        findPinned<NOTATION::COLOR::color::black, 56>,
        findPinned<NOTATION::COLOR::color::black, 57>,
        findPinned<NOTATION::COLOR::color::black, 58>,
        findPinned<NOTATION::COLOR::color::black, 59>,
        findPinned<NOTATION::COLOR::color::black, 60>,
        findPinned<NOTATION::COLOR::color::black, 61>,
        findPinned<NOTATION::COLOR::color::black, 62>,
        findPinned<NOTATION::COLOR::color::black, 63>,
};

std::vector<ExtendedMove> MoveGenerator::generate(const Board& board,
	NOTATION::COLOR::color c)
{
	std::vector<ExtendedMove> allMoves;
	ctx.board = &board;
	ctx.pieceColor = c;
    ctx.kingPosition = CheckChecker::findKing(board, c);

    ctx.Nbeatings = 0;
    ctx.Npromotions = 0;
    ctx.NUsualMoves = 0;
    ctx.NkingMoves = 0;

    auto isChecked = CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, ctx.kingPosition);
    if (c == NOTATION::COLOR::color::white)
    {
        if (isChecked)
        {
            evaluateForCheckedPosition<NOTATION::COLOR::color::white>();
        }
        else
        {
            auto pinnedFiels = whiteLookup[ctx.kingPosition]();
            evaluateNotCheckedPostions<NOTATION::COLOR::color::white>(pinnedFiels);
        }
    }
	else
    {
	    if(isChecked)
        {
	        evaluateForCheckedPosition<NOTATION::COLOR::color::black>();
        }
	    else
        {
            auto pinnedFiels = blackLookup[ctx.kingPosition]();
            evaluateNotCheckedPostions<NOTATION::COLOR::color::black>(pinnedFiels);
        }
    }

    allMoves.reserve(ctx.NkingMoves + ctx.NUsualMoves + ctx.Npromotions + ctx.Nbeatings);
	allMoves.insert(allMoves.end(), ctx.beatings, ctx.beatings + ctx.Nbeatings);
    allMoves.insert(allMoves.end(), ctx.promotions, ctx.promotions + ctx.Npromotions);
    allMoves.insert(allMoves.end(), ctx.usualMoves, ctx.usualMoves + ctx.NUsualMoves);
    allMoves.insert(allMoves.end(), ctx.kingMoves, ctx.kingMoves + ctx.NkingMoves);

    return allMoves;
}

std::vector<ExtendedMove> MoveGenerator::generate(const Board& board)
{
    return generate(board, board.playerOnMove);
}

}  // namespace MoveGenerator
