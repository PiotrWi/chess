#include "CachedMoveGeneratorMap.hpp"

#include <algorithm>
#include <utility>

static int mapToValue(unsigned char field)
{
    switch (field & NOTATION::PIECES::PIECES_MASK)
    {
        case NOTATION::PIECES::PAWN: return 100;
        case NOTATION::PIECES::KNIGHT: return 300;
        case NOTATION::PIECES::BISHOP: return 300;
        case NOTATION::PIECES::ROCK: return 500;
        case NOTATION::PIECES::QUEEN: return 900;
    }
    return  0;
}

static int mapToMovePriority(unsigned char field)
{
    switch (field & NOTATION::PIECES::PIECES_MASK)
    {
        case NOTATION::PIECES::PAWN: return 2;
        case NOTATION::PIECES::KNIGHT: return 5;
        case NOTATION::PIECES::BISHOP: return 6;
        case NOTATION::PIECES::ROCK: return 4;
        case NOTATION::PIECES::QUEEN: return 3;
        case NOTATION::PIECES::KING: return 1;
    }
    return  0;
}

const std::vector<ExtendedMove>* CachedMoveGeneratorMap::generate(const BoardEngine &be)
{
    auto hash = be.getHash();

    auto elem = cache_.get(hash, be.board);
    if (!elem)
    {
        auto moves = be.generateMoves();
        std::sort(moves.begin(), moves.end(), [](auto&& lhs, auto&& rhs){
            return std::tuple<const int&, const int&>(mapToValue(lhs.targetPiece), mapToMovePriority(lhs.sourcePiece))
                > std::tuple<const int&, const int&>(mapToValue(rhs.targetPiece), mapToMovePriority(rhs.sourcePiece));
        });
        return cache_.store(hash, be.board, std::move(moves));
    }
    return elem;
}