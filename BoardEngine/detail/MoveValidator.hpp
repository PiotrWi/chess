#pragma once

struct Board;
struct Move;

namespace MoveValidator
{
bool validateMove(const Board&, const Move&);
}
