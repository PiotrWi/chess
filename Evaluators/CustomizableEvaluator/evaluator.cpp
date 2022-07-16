/*
 * TODO: There is a bug which causes in case of interrupt default move can be return. It can happen if short time is given.
 * */
#include <evaluatorIf.hpp>

#include <detail/bitboardslookups.hpp>
#include <publicIf/NotationConversions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace
{

struct MaterialValues
{
    int pawnValue = 100;
    int knightValue = 300;
    int bishopValue = 300;
    int rockValue = 500;
    int queenValue = 900;
    int kingValue = 300;
} materiaValues;

int multiplePawnInRankValue = -50;
int singleMoveValue = 10;

int evaluateMoveCount(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * singleMoveValue;
}

int evaluateMaterial(const Board &board, NOTATION::COLOR::color playerOnMove)
{
    int value = 0;
    value += (__builtin_popcountll(board.piecesBitSets[0].pawnsMask)
            - __builtin_popcountll(board.piecesBitSets[1].pawnsMask)) * materiaValues.pawnValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].knightsMask)
            - __builtin_popcountll(board.piecesBitSets[1].knightsMask)) * materiaValues.knightValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].bishopsMask)
            - __builtin_popcountll(board.piecesBitSets[1].bishopsMask)) *materiaValues. bishopValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].rocksMask)
            - __builtin_popcountll(board.piecesBitSets[1].rocksMask)) * materiaValues.rockValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].queensMask)
            - __builtin_popcountll(board.piecesBitSets[1].queensMask)) * materiaValues.queenValue;
    // no need to evaluate kings. It do not change the value of evaluation

    if (playerOnMove == NOTATION::COLOR::color::black)
        return -1 * value;
    return value;
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

}  // namespace

void init(const char* configurationFileLocation)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(configurationFileLocation, tree);

    materiaValues.pawnValue = tree.get("COEFFICIENTS.MATERIAL.PAWN", 0);
    materiaValues.knightValue = tree.get("COEFFICIENTS.MATERIAL.KNIGHT", 0);
    materiaValues.bishopValue = tree.get("COEFFICIENTS.MATERIAL.BISHOP", 0);
    materiaValues.rockValue = tree.get("COEFFICIENTS.MATERIAL.ROCK", 0);
    materiaValues.queenValue = tree.get("COEFFICIENTS.MATERIAL.QUEEN", 0);
    materiaValues.kingValue = tree.get("COEFFICIENTS.MATERIAL.KING", 0);

    multiplePawnInRankValue = tree.get("COEFFICIENTS.PAWN_STRUCTURE.DOUBLED_PAWN_PENALITY", 0);
    singleMoveValue = tree.get("COEFFICIENTS.MOBILITY.MOVE_COUNT", 0);
}

int evaluatePosition(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be .generateValidMoveCount(be.board.playerOnMove + 1);
    return evaluateMaterial(be.board, be.board.playerOnMove)
        + evaluateMoveCount(validMovesCount, oponentValidMoves)
        + evaluatePawnStructure(be.board, be.board.playerOnMove);
}
