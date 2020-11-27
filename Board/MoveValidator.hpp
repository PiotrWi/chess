#pragma once

class Board;
class Move;

namespace MoveValidator
{
bool validateMove(const Board&, const Move&);
}
