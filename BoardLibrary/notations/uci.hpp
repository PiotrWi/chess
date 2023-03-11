#pragma once

#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include "core/Board.hpp"

namespace notations
{
namespace uci
{

ExtendedMove createExtendedMove (const std::string& moveStr,
                                 NOTATION::COLOR::color playerOnMove, const Board& board);

std::vector<char> createMoveStr(const ExtendedMove&);

}  // namespace uci
}  // namespace notations
