#pragma once

#include <publicIf/Notation.hpp>

namespace notations
{

constexpr unsigned char writablePieces[] = {NOTATION::PIECES::QUEEN,
                                            NOTATION::PIECES::BISHOP,
                                            NOTATION::PIECES::ROCK,
                                            NOTATION::PIECES::KNIGHT,
                                            NOTATION::PIECES::KING};
unsigned char createPiece(const char pieceSign, NOTATION::COLOR::color playerOnMove);
char pieceToLiteral(unsigned char field);

}
