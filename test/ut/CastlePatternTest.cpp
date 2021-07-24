#include <gtest/gtest.h>

#include <publicIf/Board.hpp>
#include <utils/BoardGenerationUtils.hpp>

TEST(CastlePatternShall, returnNonForCaslePositions)
{
    Board whiteCastles[10] {
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "     ♙♙♙"
            "      ♔ "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "     ♙♙♙"
            "       ♔"),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "       ♙"
            "     ♙♙ "
            "      ♔ "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "       ♙"
            "     ♙♙ "
            "       ♔"),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙♙♙     "
            "  ♔     "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙♙♙     "
            " ♔      "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙♙♙     "
            "♔       "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙       "
            " ♙♙     "
            "  ♔     "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙       "
            " ♙♙     "
            " ♔      "),
        utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙       "
            " ♙♙     "
            "♔       "),
    };

    for (int i = 0; i < 10; ++i)
    {
        std::cout << "whitePatterns[" << i << "].pawnsPattern = "
            << whiteCastles[i].piecesBitSets[0].pawnsMask << std::endl;
        std::cout << "whitePatterns[" << i << "].kingsMask = "
            << whiteCastles[i].piecesBitSets[0].kingsMask << std::endl;
    }
    Board blackCastles[10] {
        utils::createBoard(
            "      ♚ "
            "     ♟♟♟"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "       ♚"
            "     ♟♟♟"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "      ♚ "
            "     ♟♟ "
            "       ♟"
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "       ♚"
            "     ♟♟ "
            "       ♟"
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "  ♚     "
            "♟♟♟     "
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            " ♚      "
            "♟♟♟     "
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "♚       "
            "♟♟♟     "
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "  ♚     "
            " ♟♟     "
            "♟       "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            " ♚      "
            " ♟♟     "
            "♟       "
            "        "
            "        "
            "        "
            "        "
            "        "),
        utils::createBoard(
            "♚       "
            " ♟♟     "
            "♟       "
            "        "
            "        "
            "        "
            "        "
            "        "),
    };
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "blackPatterns[" << i << "].pawnsPattern = "
                  << blackCastles[i].piecesBitSets[1].pawnsMask << std::endl;
        std::cout << "blackPatterns[" << i << "].kingsMask = "
                  << blackCastles[i].piecesBitSets[1].kingsMask << std::endl;
    }
}