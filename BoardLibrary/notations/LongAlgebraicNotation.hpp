#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include "core/Board.hpp"

namespace notations
{
namespace long_algebraic
{

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
    NOTATION::COLOR::color playerOnMove, const Board& board);

/*!
 * \brief Create move string in complete long agebraic notation.
 *
 * \param extendedMove complete move
 *
 * \return string in complete long agebraic notation
 */
std::vector<char> createMoveStr(const ExtendedMove& extendedMove);

}  // namespace long_algebraic
}  // namespace notations

std::ostream& operator<<(std::ostream& os, const ExtendedMove&);
