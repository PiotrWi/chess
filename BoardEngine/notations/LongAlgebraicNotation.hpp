#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <publicIf/Board.hpp>

// Long algebraic notation
Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove);
std::vector<char> createMoveStr(const Move&);

std::ostream& operator<<(std::ostream& os, const Move&);

/*!
 * \brief Create extended move from string.
 * NOTE: To be used in GUI. In UT createExtendedMove is recomended.
 *
 * \param moveStr in long agebraic notation with simpifield version: a1-b1. It can contain promotion (a1-b1=Q), but figure in prefix(Qa1-b1) or beating mark(a1xb1 bit not be present)
 *
 * \param playerOnMove player which will apply this move. Needed fot castles.
 *
 * \param board before move is applied
 *
 * \return ExtendedMove with all information about move.
 */
ExtendedMove createExtendedMoveFromSimpleStr (const std::string& moveStr,
    NOTATION::COLOR::color playerOnMove, const Board& board);

/*!
 * \brief Create extended move from complete string od Long algebraic notation.
 * Syntax is like: Bb5xc6 d7d6
 *
 * \param moveStr in complete long agebraic notation. All info like: movedPiece, brating bit, promotion need to be in string.
 *
 * \param playerOnMove player which will apply this moveNeeded fot castles.
 *
 * \param target field.
 *
 * \return
 */
ExtendedMove createExtendedMove (const std::string& moveStr,
    NOTATION::COLOR::color playerOnMove, unsigned char targetField);

/*!
 * \brief Create move string in complete long agebraic notation.
 *
 * \param extendedMove complete move
 *
 * \return string in complete long agebraic notation
 */
std::vector<char> createMoveStr(const ExtendedMove& extendedMove);


std::ostream& operator<<(std::ostream& os, const ExtendedMove&);
