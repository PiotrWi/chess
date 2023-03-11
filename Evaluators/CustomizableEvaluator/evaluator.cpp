#include <iostream>
#include <evaluatorIf.hpp>
#include <Common/MatherialEvaluator.hpp>
#include <Common/PawnStructureEvaluator.hpp>
#include <Common/SquareTablesEvaluator.hpp>

#include "core/bitboards/bitboardslookups.hpp"
#include "core/NotationConversions.hpp"
#include <MoveGeneratorV2/MoveGeneratorV2.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


namespace
{

matherial_evaluator::VALUES_TYPE piecesValues;
PawnStructureCoefficients pawnStructureCoeffincients = {-50,-50, 50};
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
    try
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(configurationFileLocation, tree);

        piecesValues = matherial_evaluator::VALUES_TYPE(
                    TPawnValue(tree.get<int>("COEFFICIENTS.MATERIAL.PAWN")),
                    TBishopValue(tree.get<int>("COEFFICIENTS.MATERIAL.BISHOP")),
                    TKnightValue(tree.get<int>("COEFFICIENTS.MATERIAL.KNIGHT")),
                    TRockValue(tree.get<int>("COEFFICIENTS.MATERIAL.ROCK")),
                    TQueenValue(tree.get<int>("COEFFICIENTS.MATERIAL.QUEEN")));

        pawnStructureCoeffincients.multiplePawnInRank =  tree.get<int>("COEFFICIENTS.PAWN_STRUCTURE.DOUBLED_PAWN_PENALITY");
        pawnStructureCoeffincients.isolatedPawn =  tree.get<int>("COEFFICIENTS.PAWN_STRUCTURE.ISOLATED_PAWN_PENALITY");
        pawnStructureCoeffincients.passedPawn =  tree.get<int>("COEFFICIENTS.PAWN_STRUCTURE.PASSED_PAWN");

        singleMoveValue = tree.get<int>("COEFFICIENTS.MOBILITY.MOVE_COUNT");
        dualBishopPremium = tree.get<int>("COEFFICIENTS.DUAL_BISHOP_PREMIUM");

        for (char c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
            for (char l: {'1', '2', '3', '4', '5', '6', '7', '8'})
            {
                auto fieldStr = std::string("") + c + l;
                auto xmlKey = std::string("COEFFICIENTS.PIECE_SQUARE_TABLES.PAWN.") + fieldStr;
                squareTables.white_pawn[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get<int>(xmlKey.c_str());

                xmlKey = std::string("COEFFICIENTS.PIECE_SQUARE_TABLES.KNIGHT.") + fieldStr;
                squareTables.white_knight[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get<int>(xmlKey.c_str());

                xmlKey = std::string("COEFFICIENTS.PIECE_SQUARE_TABLES.KING.") + fieldStr;
                squareTables.white_king[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get<int>(xmlKey.c_str());

                xmlKey = std::string("COEFFICIENTS.PIECE_SQUARE_TABLES.BISHOP.") + fieldStr;
                squareTables.white_bishop[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get<int>(xmlKey.c_str());

                xmlKey = std::string("COEFFICIENTS.PIECE_SQUARE_TABLES.ROCK.") + fieldStr;
                squareTables.white_rock[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get<int>(xmlKey.c_str());

                xmlKey = std::string("COEFFICIENTS.PIECE_SQUARE_TABLES.QUEEN.") + fieldStr;
                squareTables.white_queen[NotationConversions::getFieldNum(fieldStr.c_str())] = tree.get<int>(xmlKey.c_str());
            }
        }
        reverseTable(squareTables.white_pawn, squareTables.black_pawn);
        reverseTable(squareTables.white_knight, squareTables.black_knight);
        reverseTable(squareTables.white_king, squareTables.black_king);
        reverseTable(squareTables.white_bishop, squareTables.black_bishop);
        reverseTable(squareTables.white_rock, squareTables.black_rock);
        reverseTable(squareTables.white_queen, squareTables.black_queen);
    }
    catch(std::exception& ex)
    {
        std::cerr << "exception throw while reading xml with coefficient configuration: " << configurationFileLocation << std::endl;
        std::cerr << "ex: " << ex.what() << std::endl;
        std::cerr << "no recovery has sense here."<< std::endl;
        throw;
    }
}

int evaluatePosition(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be.getMoveGeneratorV2(be.board.playerOnMove + 1).getValidMoveCount();

    return matherial_evaluator::evaluate(be.board, be.board.playerOnMove, piecesValues)
        + evaluateMoveCount(validMovesCount, oponentValidMoves)
        + pawn_structure_evaluator::evaluatePawnStructure(be.board, be.board.playerOnMove, pawnStructureCoeffincients)
        + evaluateDualBishop(be.board, be.board.playerOnMove)
        + square_tables_evaluator::evaluate(be.board, be.board.playerOnMove, squareTables);
}
