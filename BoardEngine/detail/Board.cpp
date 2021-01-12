#include "publicIf/Board.hpp"

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <publicIf/NotationConversions.hpp>

///////////////////////////////////
// MOVE
///////////////////////////////////
Move::Move() noexcept
	: source(0)
	, destination(0)
	, isPromoted(false)
	, promoteTo(0) {}

Move::Move(unsigned char s, unsigned char d, bool isPromoted, unsigned char promoteTo) noexcept
	: source(s)
	, destination(d)
	, isPromoted(isPromoted)
	, promoteTo(promoteTo) {}

Move::Move(const char* sourceStr, const char* destinationStr, bool isPromoted, unsigned char promoteTo) noexcept
	: source(NotationConversions::getFieldNum(sourceStr))
	, destination(NotationConversions::getFieldNum(destinationStr))
	, isPromoted(isPromoted)
	, promoteTo(promoteTo) {}

bool operator==(const Move& lfs, const Move& rhs) noexcept
{
	return *reinterpret_cast<const uint32_t*>(&lfs) == *reinterpret_cast<const uint32_t*>(&rhs);
}

///////////////////////////////////
// Extended Move
///////////////////////////////////

ExtendedMove::operator Move() const
{
    return Move(source, destination, flags & ExtendedMove::promotionMask, promoting);
}

bool operator ==(const ExtendedMove& lhs, const ExtendedMove& rhs) noexcept
{
    return lhs.source == rhs.source
        and lhs.destination == rhs.destination
        and lhs.flags == rhs.flags
        and lhs.promoting == rhs.promoting;
}

ExtendedMove convertMoveToExtended(const Board& board, const Move& move) noexcept
{
    const unsigned char& target = board[move.destination];
    const unsigned char& source = board[move.source] & NOTATION::PIECES::PIECES_MASK;

    unsigned char flags = ((move.isPromoted == true) ? ExtendedMove::promotionMask : 0u)
            | ((target != 0u) ? ExtendedMove::beatingMask : 0u)
            | ((source == NOTATION::PIECES::PAWN) ? ExtendedMove::pawnMoveMask : 0u)
            | ((source == NOTATION::PIECES::KING) ? ExtendedMove::kingMoveMask : 0u);

    return ExtendedMove{move.source, move.destination, flags, move.promoteTo, source, target};
}
ExtendedMove::ExtendedMove(unsigned char sourceIn,
             unsigned char destinationIn,
             unsigned char flagsIn,
             unsigned char promotingIn,
             unsigned char sourcePieceIn,
             unsigned char targetPieceIn) noexcept
    : source(sourceIn)
    , destination(destinationIn)
    , flags(flagsIn)
    , promoting(promotingIn)
    , sourcePiece(sourcePieceIn)
    , targetPiece(targetPieceIn) {}

ExtendedMove ExtendedMove::whiteShortCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e1"),
                        NotationConversions::getFieldNum("g1"),
                        0,
                        ExtendedMove::kingMoveMask,
                        NOTATION::PIECES::KING, 0};
}

ExtendedMove ExtendedMove::whiteLongCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e1"),
                        NotationConversions::getFieldNum("c1"),
                        0,
                        ExtendedMove::kingMoveMask,
                        NOTATION::PIECES::KING, 0};
}

ExtendedMove ExtendedMove::blackShortCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e8"),
                        NotationConversions::getFieldNum("g8"),
                        0,
                        ExtendedMove::kingMoveMask,
                        NOTATION::PIECES::KING, 0};
}

ExtendedMove ExtendedMove::blackLongCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e8"),
                        NotationConversions::getFieldNum("c8"),
                        0,
                        ExtendedMove::kingMoveMask,
                        NOTATION::PIECES::KING, 0};
}

///////////////////////////////////
// Board
///////////////////////////////////

unsigned char& Board::operator[](const char* field) noexcept
{
	return fields[NotationConversions::getFieldNum(field)];
}

unsigned char& Board::operator[](const unsigned char field) noexcept
{
	return fields[field];
}

const unsigned char& Board::operator[](const char* field) const noexcept
{
	return const_cast<Board&>(*this)[field];
}

const unsigned char& Board::operator[](const unsigned char field) const noexcept
{
	return const_cast<Board&>(*this)[field];
}

bool operator==(const Board& lhs, const Board& rhs) noexcept
{
    return lhs.validEnPassant == rhs.validEnPassant and lhs.playerOnMove == rhs.playerOnMove
        and lhs.castlingRights == rhs.castlingRights
        and std::equal(std::begin(lhs.fields), std::end(lhs.fields), std::begin(rhs.fields));
}

void initDefault(Board& board) noexcept
{
    board.castlingRights = NOTATION::CASTLING_RIGHTS::CASTLING_MASK;
    board.validEnPassant = -1;

    board["a1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;
    board["b1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
    board["c1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
    board["d1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN;
    board["e1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KING;
    board["f1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
    board["g1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
    board["h1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;

    char field[3] = "a2";
    for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
    {
            field[0] = c;
            board[field] = NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN;
    }

    board["a8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;
    board["b8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
    board["c8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
    board["d8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN;
    board["e8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KING;
    board["f8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
    board["g8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
    board["h8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;

    field[1] = '7';
    for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
    {
            field[0] = c;
            board[field] = NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN;
    }
}
