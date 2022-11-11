#include "MoveGenerator.hpp"
#include "MoveGeneratorContext.hpp"

#include "MoveGeneratorBackend.hpp"

#include <publicIf/NotationConversions.hpp>
#include <detail/CheckChecker.hpp>
#include <detail/PinnedMovesChecker.hpp>
#include "Strategy.hpp"

#include <detail/BitBoardsUtils.h>

namespace MoveGenerator
{

template<NOTATION::COLOR::color c>
void generateCasles();

template<>
void generateCasles<NOTATION::COLOR::color::white>()
{
    constexpr auto index = static_cast<unsigned char>(NOTATION::COLOR::color::white);
    if (ctx.board->piecesBitSets[index].kingsMask == (1ull << 4))
    {
        using namespace NOTATION::CASTLING_RIGHTS;
        if (ctx.board->castlingRights & WHITE_LONG_BIT
            && (ctx.board->piecesBitSets[index].rocksMask & (1ull))
            && (ctx.allPieces & ((1ull << 3) | (1ull << 2) | (1ull << 1))) == 0
            && ! CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 3))
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>::addCastling(4, 2);
        }

        if (ctx.board->castlingRights & WHITE_SHORT_BIT
            && (ctx.board->piecesBitSets[index].rocksMask & (1ull << 7))
            && (ctx.allPieces & ((1ull << 5) | (1ull << 6))) == 0
            && ! CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 5))
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>::addCastling(4, 6);
        }
    }
}

template<>
void generateCasles<NOTATION::COLOR::color::black>()
{
    constexpr auto index = static_cast<unsigned char>(NOTATION::COLOR::color::black);
    if (ctx.board->piecesBitSets[index].kingsMask == (1ull << 60))
    {
        using namespace NOTATION::CASTLING_RIGHTS;
        if (ctx.board->castlingRights & BLACK_LONG_BIT
            && (ctx.board->piecesBitSets[index].rocksMask & (1ull << 56))
            && (ctx.allPieces & ((1ull << 59) | (1ull << 58) | (1ull << 57))) == 0
            && ! CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 59))
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>::addCastling(60, 58);
        }

        if (ctx.board->castlingRights & BLACK_SHORT_BIT
            && (ctx.board->piecesBitSets[index].rocksMask & (1ull << 63))
            && (ctx.allPieces & ((1ull << 61) | (1ull << 62))) == 0
            && ! CheckChecker::isAttackedOn(*ctx.board, ctx.pieceColor, 61))
        {
            StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>::addCastling(60, 62);
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

        constexpr auto colorNum = static_cast<unsigned char>(NOTATION::COLOR::color::white);
        auto enPassantBitBoard = (1ull << (ctx.board)->validEnPassant);
        if (((enPassantBitBoard & NOT_H_COL) >> 7) & ctx.board->piecesBitSets[colorNum].pawnsMask)
        {
            TMoveAddingStrategy::addEnPasantBeating((ctx.board)->validEnPassant - 7, (ctx.board)->validEnPassant,
                                                    NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK);
        }
        if (((enPassantBitBoard & NOT_A_COL) >> 9) & ctx.board->piecesBitSets[colorNum].pawnsMask)
        {
            TMoveAddingStrategy::addEnPasantBeating((ctx.board)->validEnPassant - 9, (ctx.board)->validEnPassant,
                                                    NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK);
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

        constexpr auto colorNum = static_cast<unsigned char>(NOTATION::COLOR::color::black);
        auto enPassantBitBoard = (1ull << (ctx.board)->validEnPassant);
        if (((enPassantBitBoard & NOT_H_COL) << 9) & ctx.board->piecesBitSets[colorNum].pawnsMask)
        {
            TMoveAddingStrategy::addEnPasantBeating((ctx.board)->validEnPassant + 9, (ctx.board)->validEnPassant,
                                                    NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE);
        }
        if (((enPassantBitBoard & NOT_A_COL) << 7) & ctx.board->piecesBitSets[colorNum].pawnsMask)
        {
            TMoveAddingStrategy::addEnPasantBeating((ctx.board)->validEnPassant + 7, (ctx.board)->validEnPassant,
                                                    NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE);
        }
    }
};

static void generateImpl(Board& board,
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

    ctx.allPieces = getAllOccupiedFields(board);
    ctx.opponentPieces = getAllOccupiedPerColor(board, c+1);

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
            auto pinnedFields = findPinned(board, NOTATION::COLOR::color::white, ctx.kingPosition); // TODO optimizations
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
            auto pinnedFields = findPinned(board, NOTATION::COLOR::color::black, ctx.kingPosition); // TODO optimizations
            NoLookup::evaluateNotCheckedPostions<NOTATION::COLOR::color::black>(pinnedFields);
            generateCasles<NOTATION::COLOR::color::black>();
            GenerateEnPasant<NOTATION::COLOR::color::black,
                    StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>>::proccess();
        }
    }
}

std::vector<ExtendedMove> MoveGenerator::generate(Board& board,
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

std::vector<ExtendedMove> MoveGenerator::generate(Board& board)
{
    return generate(board, board.playerOnMove);
}

unsigned MoveGenerator::getMoveCount(Board& board
        , NOTATION::COLOR::color c)
{
    generateImpl(board, c);
    return ctx.NKingMoves + ctx.NFiguresMoves + ctx.NPawnsMoves + ctx.Npromotions + ctx.Nbeatings;
}

unsigned MoveGenerator::getMoveCount(Board& board)
{
    return getMoveCount(board, board.playerOnMove);
}

}  // namespace MoveGenerator
