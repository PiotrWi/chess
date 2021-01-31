#include "MoveGenerator.hpp"
#include "MoveGeneratorContext.hpp"

#include "MoveGeneratorBackend.hpp"

#include <publicIf/NotationConversions.hpp>
#include <detail/CheckChecker.hpp>
#include <detail/PinnedMovesChecker.hpp>
#include "Strategy.hpp"

namespace MoveGenerator
{

template<NOTATION::COLOR::color c>
void generateCasles();

template<>
void generateCasles<NOTATION::COLOR::color::white>()
{
    if ((*ctx.board)[4] == (NOTATION::PIECES::KING | NOTATION::COLOR::WHITE))
    {
        using namespace NOTATION::CASTLING_RIGHTS;
        if (ctx.board->castlingRights & WHITE_LONG_BIT
            and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 3)
            and (*ctx.board)[3] == 0
            and (*ctx.board)[2] == 0
            and (*ctx.board)[1] == 0)
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>::addKing(4, 2);
        }

        if (ctx.board->castlingRights & WHITE_SHORT_BIT
            and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 5)
            and (*ctx.board)[5] == 0
            and (*ctx.board)[6] == 0)
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>::addKing(4, 6);
        }
    }
}

template<>
void generateCasles<NOTATION::COLOR::color::black>()
{
    if ((*ctx.board)[60] == (NOTATION::PIECES::KING | NOTATION::COLOR::BLACK))
    {
        using namespace NOTATION::CASTLING_RIGHTS;
        if (ctx.board->castlingRights & BLACK_LONG_BIT
            and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 59)
            and (*ctx.board)[59] == 0
            and (*ctx.board)[58] == 0
            and (*ctx.board)[57] == 0)
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>::addKing(60, 58);
        }

        if (ctx.board->castlingRights & BLACK_SHORT_BIT
            and not CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 61)
            and (*ctx.board)[61] == 0
            and (*ctx.board)[62] == 0)
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>::addKing(60, 62);
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
                                                        NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK);
            }
        }
        if (enPassantCol > 0u)
        {
            auto whitePawnLocCandidate = (ctx.board)->validEnPassant - NOTATION::COORDINATES::ROW_DIFF - 1;
            if ((*ctx.board)[whitePawnLocCandidate] ==
                (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
            {
                TMoveAddingStrategy::addPawnWithBeating(whitePawnLocCandidate, (ctx.board)->validEnPassant,
                                                        NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK);
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
                                                        NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE);
            }
        }
        if (enPassantCol > 0u)
        {
            auto whitePawnLocCandidate = (ctx.board)->validEnPassant + NOTATION::COORDINATES::ROW_DIFF - 1;
            if ((*ctx.board)[whitePawnLocCandidate] ==
                (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
            {
                TMoveAddingStrategy::addPawnWithBeating(whitePawnLocCandidate, (ctx.board)->validEnPassant,
                                                        NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE);
            }
        }
    }
};

static void generateImpl(const Board& board,
                         NOTATION::COLOR::color c)
{
    ctx.board = &board;
    ctx.pieceColor = c;
    ctx.kingPosition = CheckChecker::findKing(board, c);

    ctx.Nbeatings = 0;
    ctx.Npromotions = 0;
    ctx.NFiguresMoves = 0;
    ctx.NPawnsMoves = 0;
    ctx.NKingMoves = 0;

    auto isChecked = CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, ctx.kingPosition);
    if (c == NOTATION::COLOR::color::white)
    {
        if (isChecked)
        {
            NoLookup::evaluateForCheckedPosition<NOTATION::COLOR::color::white>();
            GenerateEnPasant<NOTATION::COLOR::color::white,
                StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>>::proccess();
        }
        else
        {
            auto pinnedFields = findPinned<NOTATION::COLOR::color::white>(board, ctx.kingPosition);
            NoLookup::evaluateNotCheckedPostions<NOTATION::COLOR::color::white>(pinnedFields);
            generateCasles<NOTATION::COLOR::color::white>();
            GenerateEnPasant<NOTATION::COLOR::color::white,
                    StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>>::proccess();
        }
    }
    else
    {
        if(isChecked)
        {
            NoLookup::evaluateForCheckedPosition<NOTATION::COLOR::color::black>();
            GenerateEnPasant<NOTATION::COLOR::color::black,
                    StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>>::proccess();
        }
        else
        {
            auto pinnedFields = findPinned<NOTATION::COLOR::color::black>(board, ctx.kingPosition);
            NoLookup::evaluateNotCheckedPostions<NOTATION::COLOR::color::black>(pinnedFields);
            generateCasles<NOTATION::COLOR::color::black>();
            GenerateEnPasant<NOTATION::COLOR::color::black,
                    StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>>::proccess();        }
    }
}

std::vector<ExtendedMove> MoveGenerator::generate(const Board& board,
	NOTATION::COLOR::color c)
{
    generateImpl(board, c);
	std::vector<ExtendedMove> allMoves;

    allMoves.reserve(ctx.NKingMoves + ctx.NFiguresMoves + ctx.NPawnsMoves + ctx.Npromotions + ctx.Nbeatings);
	allMoves.insert(allMoves.end(), ctx.beatings, ctx.beatings + ctx.Nbeatings);
    allMoves.insert(allMoves.end(), ctx.promotions, ctx.promotions + ctx.Npromotions);
    allMoves.insert(allMoves.end(), ctx.figuresMoves, ctx.figuresMoves + ctx.NFiguresMoves);
    allMoves.insert(allMoves.end(), ctx.pawnsMoves, ctx.pawnsMoves + ctx.NPawnsMoves);
    allMoves.insert(allMoves.end(), ctx.kingMoves, ctx.kingMoves + ctx.NKingMoves);

    return allMoves;
}

std::vector<ExtendedMove> MoveGenerator::generate(const Board& board)
{
    return generate(board, board.playerOnMove);
}

unsigned MoveGenerator::getMoveCount(const Board& board
        , NOTATION::COLOR::color c)
{
    generateImpl(board, c);
    return ctx.NKingMoves + ctx.NFiguresMoves + ctx.NPawnsMoves + ctx.Npromotions + ctx.Nbeatings;
}

unsigned MoveGenerator::getMoveCount(const Board& board)
{
    return getMoveCount(board, board.playerOnMove);
}

}  // namespace MoveGenerator
