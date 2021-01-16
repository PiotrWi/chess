#include "MoveGenerator.hpp"
#include <common/evaluators/MaterialAndMoveCountEvaluator.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

const std::vector<ExtendedMove> MoveGenerator::generate(const BoardEngine& be, unsigned char) const
{
    return be.generateMoves();
}

const std::vector<ExtendedMove> MoveGenerator::generate(const BoardEngine& be, NOTATION::COLOR::color color, unsigned char) const
{
    return be.generateMovesFor(color);
}

int MoveGenerator::getEvaluationValue(const BoardEngine& be)
{
    return evaluateFunction(be);
}

}  // namespace move_generators
}  // namespace common
}  // namespace players
