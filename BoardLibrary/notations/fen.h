#pragma once

#include <BoardEngine.hpp>
#include <string>

namespace notations
{
namespace fen
{

BoardEngine initByFen(std::string fenStr);

}  // namespace fen
}  // namespace notations
