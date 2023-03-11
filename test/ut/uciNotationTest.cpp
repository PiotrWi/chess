#include <string>

#include <notations/uci.hpp>
#include "core/NotationConversions.hpp"

#include <gtest/gtest.h>

TEST(FenShall, CorrectlyPresentPromotion)
{
    // "h2g1"
    ExtendedMove move;
    move.source = NotationConversions::getFieldNum("h2");
    move.destination = NotationConversions::getFieldNum("g1");
    move.flags = ExtendedMove::promotionMask | ExtendedMove::beatingMask | ExtendedMove::pawnMoveMask;
    move.promoting = NOTATION::PIECES::QUEEN | NOTATION::COLOR::BLACK;
    move.sourcePiece = NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK;
    move.targetPiece = NOTATION::PIECES::ROCK | NOTATION::COLOR::WHITE;
    ASSERT_EQ(std::string(notations::uci::createMoveStr(move).data()), std::string("h2g1q"));
}