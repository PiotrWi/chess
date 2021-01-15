#include "MoveGenerator.hpp"


namespace players
{
namespace common
{
namespace move_generators
{


const std::vector<ExtendedMove> MoveGenerator::generate(const BoardEngine& be) const
{
    return be.generateMoves();
}

const std::vector<ExtendedMove> MoveGenerator::generate(const BoardEngine& be, NOTATION::COLOR::color color) const
{
    return be.generateMovesFor(color);
}

unsigned MoveGenerator::getMoveCount(const BoardEngine& be) const
{
    return be.generateValidMoveCount();
}

unsigned MoveGenerator::getMoveCount(const BoardEngine& be, NOTATION::COLOR::color color) const
{
    return be.generateValidMoveCount(color);
}


}  // namespace move_generators
}  // namespace common
}  // namespace players
