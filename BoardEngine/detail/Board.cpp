#include "publicIf/Board.hpp"

#include <algorithm>
#include <ostream>
#include <publicIf/NotationConversions.hpp>

#include <cassert>

bool Board::PlayerBitMasksSet::operator==(const PlayerBitMasksSet& rhs) const
{
    return this->rocksMask == rhs.rocksMask
        && this->knightsMask == rhs.knightsMask
        && this->kingsMask == rhs.kingsMask
        && this->bishopsMask == rhs.bishopsMask
        && this->queensMask == rhs.queensMask
        && this->pawnsMask == rhs.pawnsMask;
}

unsigned char Board::getField(const unsigned char field) const noexcept
{
#ifdef ASSERTSON
    assert(field < 64);
#endif

    u_int64_t pieceBit = 0;
    pieceBit |= ((piecesBitSets[WHITE_INDEX].pawnsMask >> field) & 0b1u) << 0;
    pieceBit |= ((piecesBitSets[WHITE_INDEX].rocksMask >> field) & 0b1u) << 1;
    pieceBit |= ((piecesBitSets[WHITE_INDEX].knightsMask >> field) & 0b1u) << 2;
    pieceBit |= ((piecesBitSets[WHITE_INDEX].bishopsMask >> field) & 0b1u) << 3;
    pieceBit |= ((piecesBitSets[WHITE_INDEX].queensMask >> field) & 0b1u) << 4;
    pieceBit |= ((piecesBitSets[WHITE_INDEX].kingsMask >> field) & 0b1u) << 5;

    pieceBit |= ((piecesBitSets[BLACK_INDEX].pawnsMask >> field) & 0b1u) << 6;
    pieceBit |= ((piecesBitSets[BLACK_INDEX].rocksMask >> field) & 0b1u) << 7;
    pieceBit |= ((piecesBitSets[BLACK_INDEX].knightsMask >> field) & 0b1u) << 8;
    pieceBit |= ((piecesBitSets[BLACK_INDEX].bishopsMask >> field) & 0b1u) << 9;
    pieceBit |= ((piecesBitSets[BLACK_INDEX].queensMask >> field) & 0b1u) << 10;
    pieceBit |= ((piecesBitSets[BLACK_INDEX].kingsMask >> field) & 0b1u) << 11;

    if (pieceBit == 0) return 0;
    static unsigned char pieceMapping[12] {
        NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN,
        NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK,
        NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT,
        NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP,
        NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN,
        NOTATION::COLOR::WHITE | NOTATION::PIECES::KING,
        NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN,
        NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK,
        NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT,
        NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP,
        NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN,
        NOTATION::COLOR::BLACK | NOTATION::PIECES::KING,
    };
    return pieceMapping[63-__builtin_clzll(pieceBit)];
}

unsigned char Board::getField(const char* field) const noexcept
{
    return getField(NotationConversions::getFieldNum(field));
}

void Board::setField(const unsigned char field, unsigned char val)
{
#ifdef ASSERTSON
    assert(field < 64);
#endif
    u_int64_t pieceBitNegation = (~(1lu << field));
    for (unsigned char i = 0; i < 2; ++i)
    {
        piecesBitSets[i].pawnsMask &= pieceBitNegation;
        piecesBitSets[i].rocksMask &= pieceBitNegation;
        piecesBitSets[i].knightsMask &= pieceBitNegation;
        piecesBitSets[i].bishopsMask &= pieceBitNegation;
        piecesBitSets[i].queensMask &= pieceBitNegation;
        piecesBitSets[i].kingsMask &= pieceBitNegation;
    }

    switch (val)
    {
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN):
            piecesBitSets[0].pawnsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK):
            piecesBitSets[0].rocksMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT):
            piecesBitSets[0].knightsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP):
            piecesBitSets[0].bishopsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN):
            piecesBitSets[0].queensMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::KING):
            piecesBitSets[0].kingsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN):
            piecesBitSets[1].pawnsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK):
            piecesBitSets[1].rocksMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT):
            piecesBitSets[1].knightsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP):
            piecesBitSets[1].bishopsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN):
            piecesBitSets[1].queensMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::KING):
            piecesBitSets[1].kingsMask |= (1ull << field);
            return;
    }
}

void Board::setField(const char* field, unsigned char val)
{
    setField(NotationConversions::getFieldNum(field), val);
}

bool operator==(const Board& lhs, const Board& rhs) noexcept
{
    return lhs.piecesBitSets[0] == rhs.piecesBitSets[0]
        && lhs.piecesBitSets[1] == rhs.piecesBitSets[1]
        && lhs.playerOnMove == rhs.playerOnMove
        && lhs.castlingRights == rhs.castlingRights
        && lhs.validEnPassant == rhs.validEnPassant;
}

void initDefault(Board& board) noexcept
{
    board.playerOnMove = NOTATION::COLOR::color::white;
    board.validEnPassant = -1;
    board.castlingRights = NOTATION::CASTLING_RIGHTS::CASTLING_MASK;

    board.piecesBitSets[0].pawnsMask =      0x00'00'00'00'00'00'FF'00ull;
    board.piecesBitSets[0].queensMask =     0x00'00'00'00'00'00'00'08ull;
    board.piecesBitSets[0].bishopsMask =    0x00'00'00'00'00'00'00'24ull;
    board.piecesBitSets[0].knightsMask =    0x00'00'00'00'00'00'00'42ull;
    board.piecesBitSets[0].kingsMask =      0x00'00'00'00'00'00'00'10ull;
    board.piecesBitSets[0].rocksMask =      0x00'00'00'00'00'00'00'81ull;

    board.piecesBitSets[1].pawnsMask =      0x00'FF'00'00'00'00'00'00ull;
    board.piecesBitSets[1].queensMask =     0x08'00'00'00'00'00'00'00ull;
    board.piecesBitSets[1].bishopsMask =    0x24'00'00'00'00'00'00'00ull;
    board.piecesBitSets[1].knightsMask =    0x42'00'00'00'00'00'00'00ull;
    board.piecesBitSets[1].kingsMask =      0x10'00'00'00'00'00'00'00ull;
    board.piecesBitSets[1].rocksMask =      0x81'00'00'00'00'00'00'00ull;

}

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
	, promoteTo(promoteTo)
{
#ifdef ASSERTSON
    assert(source < 64);
    assert(destination < 64);
#endif
}

Move::Move(const char* sourceStr, const char* destinationStr, bool isPromoted, unsigned char promoteTo) noexcept
	: source(NotationConversions::getFieldNum(sourceStr))
	, destination(NotationConversions::getFieldNum(destinationStr))
	, isPromoted(isPromoted)
	, promoteTo(promoteTo)
{
#ifdef ASSERTSON
    assert(source < 64);
    assert(destination < 64);
#endif
}

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
    const unsigned char& target = board.getField(move.destination);
    const unsigned char& source = board.getField(move.source) & NOTATION::PIECES::PIECES_MASK;

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
    , targetPiece(targetPieceIn)
{
#ifdef ASSERTSON
    assert(source < 64);
    assert(destination < 64);
    assert(sourcePiece != 0);
    assert(targetPiece != 0 || (flags & ExtendedMove::beatingMask) == 0);
#endif
}

ExtendedMove ExtendedMove::whiteShortCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e1"),
                        NotationConversions::getFieldNum("g1"),
                        ExtendedMove::kingMoveMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::WHITE, 0};
}

ExtendedMove ExtendedMove::whiteLongCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e1"),
                        NotationConversions::getFieldNum("c1"),
                        ExtendedMove::kingMoveMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::WHITE, 0};
}

ExtendedMove ExtendedMove::blackShortCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e8"),
                        NotationConversions::getFieldNum("g8"),
                        ExtendedMove::kingMoveMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::BLACK, 0};
}

ExtendedMove ExtendedMove::blackLongCaste()
{
    return ExtendedMove{NotationConversions::getFieldNum("e8"),
                        NotationConversions::getFieldNum("c8"),
                        ExtendedMove::kingMoveMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::BLACK, 0};
}
