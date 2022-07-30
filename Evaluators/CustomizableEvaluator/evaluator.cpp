/*
 * TODO: There is a bug which causes in case of interrupt default move can be return. It can happen if short time is given.
 * */
#include <evaluatorIf.hpp>
#include <Common/MatherialEvaluator.hpp>

#include <detail/bitboardslookups.hpp>
#include <publicIf/NotationConversions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <Common/MatherialEvaluator.hpp>

namespace
{

matherial_evaluator::VALUES_TYPE piecesValues;

int multiplePawnInRankValue = -50;
int singleMoveValue = 10;
int dualBishopPremium = 50;

int evaluateMoveCount(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * singleMoveValue;
}

int evaluatePawnStructureForWhite(const Board & board)
{
    auto eval = 0;

    for (auto && rank : ranks)
    {
        auto pawnsInRank = __builtin_popcountll(
            rank & board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask);
        if (pawnsInRank > 1)
        {
            eval += (pawnsInRank - 1) * multiplePawnInRankValue;
        }
    }

    return eval;
}

int evaluatePawnStructureForBlack(const Board& board)
{
    auto eval = 0;

    for (auto && rank : ranks)
    {
        auto pawnsInRank = __builtin_popcountll(
                rank & board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask);
        if (pawnsInRank > 1)
        {
            eval += (pawnsInRank - 1) * multiplePawnInRankValue;
        }
    }

    return eval;
}

int evaluatePawnStructure(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto val = evaluatePawnStructureForWhite(board) - evaluatePawnStructureForBlack(board);
    return (playerOnMove == NOTATION::COLOR::color::white)
        ? val
        : -1 * val;
}

int evaluateDualBishop(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto whiteDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask));
    auto blackDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask));
    if (playerOnMove == NOTATION::COLOR::color::black)
        return blackDualBishipPremium - whiteDualBishipPremium;
    return whiteDualBishipPremium - blackDualBishipPremium;
}

}  // namespace

void init(const char* configurationFileLocation)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(configurationFileLocation, tree);

    piecesValues = matherial_evaluator::VALUES_TYPE(
                TPawnValue(tree.get("COEFFICIENTS.MATERIAL.PAWN", 0)),
                TBishopValue(tree.get("COEFFICIENTS.MATERIAL.BISHOP", 0)),
                TKnightValue(tree.get("COEFFICIENTS.MATERIAL.KNIGHT", 0)),
                TRockValue(tree.get("COEFFICIENTS.MATERIAL.ROCK", 0)),
                TQueenValue(tree.get("COEFFICIENTS.MATERIAL.QUEEN", 0)));

    multiplePawnInRankValue = tree.get("COEFFICIENTS.PAWN_STRUCTURE.DOUBLED_PAWN_PENALITY", 0);
    singleMoveValue = tree.get("COEFFICIENTS.MOBILITY.MOVE_COUNT", 0);
    dualBishopPremium = tree.get("COEFFICIENTS.DUAL_BISHOP_PREMIUM", 0);
}

int evaluatePosition(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be.generateValidMoveCount(be.board.playerOnMove + 1);
    return matherial_evaluator::evaluate(be.board, be.board.playerOnMove, piecesValues)
        + evaluateMoveCount(validMovesCount, oponentValidMoves)
        + evaluatePawnStructure(be.board, be.board.playerOnMove)
        + evaluateDualBishop(be.board, be.board.playerOnMove);
}
