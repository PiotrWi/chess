#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <publicIf/Board.hpp>

namespace notations
{
namespace coordinates
{

Move createMove (const std::string& moveStr,
    NOTATION::COLOR::color playerOnMove);

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
ExtendedMove createExtendedMove (const std::string& moveStr,
    NOTATION::COLOR::color playerOnMove, const Board& board);

std::vector<char> createMoveStr(const Move&);

}  // namespace coordinates
}  // namespace notations

std::ostream& operator<<(std::ostream& os, const Move&);
