/*
 * TODO: There is a bug which causes in case of interrupt default move can be return. It can happen if short time is given.
 * */
#include <evaluatorIf.hpp>
#include <Common/MatherialEvaluator.hpp>
#include <Common/PawnStructureEvaluator.hpp>
#include <Common/SquareTablesEvaluator.hpp>

#include <detail/bitboardslookups.hpp>
#include <publicIf/NotationConversions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


namespace
{

matherial_evaluator::VALUES_TYPE piecesValues;
PawnStructureCoefficients pawnStructureCoeffincients = {-50,-50};
SuareTableCoeffictients squareTables;

int singleMoveValue = 10;
int dualBishopPremium = 50;

int evaluateMoveCount(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * singleMoveValue;
}

int evaluateDualBishop(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto whiteDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask));
    auto blackDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::BLACK].bishopsMask));
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

    pawnStructureCoeffincients.multiplePawnInRank =  tree.get("COEFFICIENTS.PAWN_STRUCTURE.DOUBLED_PAWN_PENALITY", 0);
    pawnStructureCoeffincients.isolatedPawn =  tree.get("COEFFICIENTS.PAWN_STRUCTURE.ISOLATED_PAWN_PENALITY", 0);

    singleMoveValue = tree.get("COEFFICIENTS.MOBILITY.MOVE_COUNT", 0);
    dualBishopPremium = tree.get("COEFFICIENTS.DUAL_BISHOP_PREMIUM", 0);

    for (char c: "abcdefgh")
    {
        for (char l: "12345678")
        {
            auto fieldStr = std::to_string(c) + std::to_string(l);
            auto xmlKey = std::string("PIECE_SQUARE_TABLES.PAWN.") + fieldStr;
            squareTables.white_pawn[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get(xmlKey.c_str(), 0);

            xmlKey = std::string("PIECE_SQUARE_TABLES.KNIGHT.") + fieldStr;
            squareTables.white_knight[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get(xmlKey.c_str(), 0);

            xmlKey = std::string("PIECE_SQUARE_TABLES.KING.") + fieldStr;
            squareTables.white_king[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get(xmlKey.c_str(), 0);

            xmlKey = std::string("PIECE_SQUARE_TABLES.BISHOP.") + fieldStr;
            squareTables.white_bishop[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get(xmlKey.c_str(), 0);

            xmlKey = std::string("PIECE_SQUARE_TABLES.ROCK.") + fieldStr;
            squareTables.white_rock[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get(xmlKey.c_str(), 0);

            xmlKey = std::string("PIECE_SQUARE_TABLES.QUEEN.") + fieldStr;
            squareTables.white_queen[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get(xmlKey.c_str(), 0);
        }
    }
    reverseTable(squareTables.white_pawn, squareTables.black_pawn);
    reverseTable(squareTables.white_knight, squareTables.black_knight);
    reverseTable(squareTables.white_king, squareTables.black_king);
    reverseTable(squareTables.white_bishop, squareTables.black_bishop);
    reverseTable(squareTables.white_rock, squareTables.black_rock);
    reverseTable(squareTables.white_queen, squareTables.black_queen);
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
        + pawn_structure_evaluator::evaluatePawnStructure(be.board, be.board.playerOnMove, pawnStructureCoeffincients)
        + evaluateDualBishop(be.board, be.board.playerOnMove)
        + square_tables_evaluator::evaluate(be.board, be.board.playerOnMove, squareTables);
}
