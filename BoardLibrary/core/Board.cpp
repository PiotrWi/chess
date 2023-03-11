#include "core/Board.hpp"

#include <ostream>
#include "core/NotationConversions.hpp"

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

    auto fieldBit = (1ull <<field);
    if (piecesBitSets[WHITE_INDEX].pawnsMask & fieldBit) return NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN;
    if (piecesBitSets[BLACK_INDEX].pawnsMask & fieldBit) return NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN;
    if (piecesBitSets[WHITE_INDEX].rocksMask & fieldBit) return NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;
    if (piecesBitSets[BLACK_INDEX].rocksMask & fieldBit) return NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;
    if (piecesBitSets[WHITE_INDEX].knightsMask & fieldBit) return NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
    if (piecesBitSets[BLACK_INDEX].knightsMask & fieldBit) return NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
    if (piecesBitSets[WHITE_INDEX].bishopsMask & fieldBit) return NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
    if (piecesBitSets[BLACK_INDEX].bishopsMask & fieldBit) return NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
    if (piecesBitSets[WHITE_INDEX].queensMask & fieldBit) return NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN;
    if (piecesBitSets[BLACK_INDEX].queensMask & fieldBit) return NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN;
    if (piecesBitSets[WHITE_INDEX].kingsMask & fieldBit) return NOTATION::COLOR::WHITE | NOTATION::PIECES::KING;
    if (piecesBitSets[BLACK_INDEX].kingsMask & fieldBit) return NOTATION::COLOR::BLACK | NOTATION::PIECES::KING;

    return 0;
}

unsigned char Board::getFieldForNonEmpty(const unsigned char field, NOTATION::COLOR::color c) const noexcept
{
#ifdef ASSERTSON
    assert(field < 64);
#endif

    unsigned char colorBit = static_cast<unsigned char>(c);
    auto fieldBit = (1ull <<field);

    if (piecesBitSets[colorBit].pawnsMask & fieldBit) return colorBit | NOTATION::PIECES::PAWN;
    if (piecesBitSets[colorBit].rocksMask & fieldBit) return colorBit | NOTATION::PIECES::ROCK;
    if (piecesBitSets[colorBit].knightsMask & fieldBit) return colorBit | NOTATION::PIECES::KNIGHT;
    if (piecesBitSets[colorBit].bishopsMask & fieldBit) return colorBit | NOTATION::PIECES::BISHOP;
    if (piecesBitSets[colorBit].queensMask & fieldBit) return colorBit | NOTATION::PIECES::QUEEN;
    return colorBit | NOTATION::PIECES::KING;
}

unsigned char Board::getField(const char* field) const noexcept
{
    return getField(NotationConversions::getFieldNum(field));
}

void Board::cleanConcrete(const unsigned char field, unsigned char oldValue) noexcept
{
    switch (oldValue)
    {
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN):
            piecesBitSets[WHITE_INDEX].pawnsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK):
            piecesBitSets[WHITE_INDEX].rocksMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT):
            piecesBitSets[WHITE_INDEX].knightsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP):
            piecesBitSets[WHITE_INDEX].bishopsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN):
            piecesBitSets[WHITE_INDEX].queensMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::KING):
            piecesBitSets[WHITE_INDEX].kingsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN):
            piecesBitSets[BLACK_INDEX].pawnsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK):
            piecesBitSets[BLACK_INDEX].rocksMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT):
            piecesBitSets[BLACK_INDEX].knightsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP):
            piecesBitSets[BLACK_INDEX].bishopsMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN):
            piecesBitSets[BLACK_INDEX].queensMask ^= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::KING):
            piecesBitSets[BLACK_INDEX].kingsMask ^= (1ull << field);
            return;
    }
#ifdef ASSERTSON
    assert(field < 64);
#endif
}

void Board::setConcrete(const unsigned char field, unsigned char value) noexcept
{
    switch (value)
    {
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN):
            piecesBitSets[WHITE_INDEX].pawnsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK):
            piecesBitSets[WHITE_INDEX].rocksMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT):
            piecesBitSets[WHITE_INDEX].knightsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP):
            piecesBitSets[WHITE_INDEX].bishopsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN):
            piecesBitSets[WHITE_INDEX].queensMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::WHITE | NOTATION::PIECES::KING):
            piecesBitSets[WHITE_INDEX].kingsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN):
            piecesBitSets[BLACK_INDEX].pawnsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK):
            piecesBitSets[BLACK_INDEX].rocksMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT):
            piecesBitSets[BLACK_INDEX].knightsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP):
            piecesBitSets[BLACK_INDEX].bishopsMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN):
            piecesBitSets[BLACK_INDEX].queensMask |= (1ull << field);
            return;
        case(NOTATION::COLOR::BLACK | NOTATION::PIECES::KING):
            piecesBitSets[BLACK_INDEX].kingsMask |= (1ull << field);
            return;
    }
}

void Board::setField(const unsigned char field, unsigned char value) noexcept
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

    setConcrete(field, value);
}

void Board::setPreviouslyEmptyField(const unsigned char field, unsigned char value) noexcept
{
#ifdef ASSERTSON
    assert(field < 64);
#endif
    setConcrete(field, value);
}
void Board::setField(const unsigned char field, unsigned char oldValue, unsigned char value) noexcept
{
    cleanConcrete(field, oldValue);
    setConcrete(field, value);
}

void Board::setField(const char* field, unsigned char val) noexcept
{
    setField(NotationConversions::getFieldNum(field), val);
}

void Board::clearField(const unsigned char field, unsigned char oldValue) noexcept
{
#ifdef ASSERTSON
    assert(field < 64);
#endif
    cleanConcrete(field, oldValue);
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

bool Board::isCorrect() const noexcept
{
    uint64_t allOccupied = piecesBitSets[0].pawnsMask | piecesBitSets[0].queensMask | piecesBitSets[0].bishopsMask |
        piecesBitSets[0].knightsMask | piecesBitSets[0].kingsMask | piecesBitSets[0].rocksMask |
        piecesBitSets[1].pawnsMask | piecesBitSets[1].queensMask | piecesBitSets[1].bishopsMask |
        piecesBitSets[1].knightsMask | piecesBitSets[1].kingsMask | piecesBitSets[1].rocksMask;
    uint64_t sanityCheck = piecesBitSets[0].pawnsMask ^ piecesBitSets[0].queensMask ^ piecesBitSets[0].bishopsMask ^
                           piecesBitSets[0].knightsMask ^ piecesBitSets[0].kingsMask ^ piecesBitSets[0].rocksMask ^
                           piecesBitSets[1].pawnsMask ^ piecesBitSets[1].queensMask ^ piecesBitSets[1].bishopsMask ^
                           piecesBitSets[1].knightsMask ^ piecesBitSets[1].kingsMask ^ piecesBitSets[1].rocksMask;
    return allOccupied == sanityCheck;
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
        && lhs.destination == rhs.destination
        && lhs.sourcePiece == rhs.sourcePiece
        && lhs.targetPiece == rhs.targetPiece
        && lhs.flags == rhs.flags
        && lhs.promoting == rhs.promoting;
}

ExtendedMove convertMoveToExtended(const Board& board, const Move& move) noexcept
{
    const unsigned char target = board.getField(move.destination);
    const unsigned char source = board.getField(move.source)
            & (NOTATION::PIECES::PIECES_MASK | NOTATION::COLOR::COLOR_MASK);

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
    assert(targetPiece == 0 || (flags & ExtendedMove::beatingMask) != 0);
#endif
}

ExtendedMove ExtendedMove::whiteShortCaste() noexcept
{
    return ExtendedMove{NotationConversions::getFieldNum("e1"),
                        NotationConversions::getFieldNum("g1"),
                        ExtendedMove::kingMoveMask | ExtendedMove::castlingMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::WHITE, 0};
}

ExtendedMove ExtendedMove::whiteLongCaste() noexcept
{
    return ExtendedMove{NotationConversions::getFieldNum("e1"),
                        NotationConversions::getFieldNum("c1"),
                        ExtendedMove::kingMoveMask | ExtendedMove::castlingMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::WHITE, 0};
}

ExtendedMove ExtendedMove::blackShortCaste() noexcept
{
    return ExtendedMove{NotationConversions::getFieldNum("e8"),
                        NotationConversions::getFieldNum("g8"),
                        ExtendedMove::kingMoveMask | ExtendedMove::castlingMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::BLACK, 0};
}

ExtendedMove ExtendedMove::blackLongCaste() noexcept
{
    return ExtendedMove{NotationConversions::getFieldNum("e8"),
                        NotationConversions::getFieldNum("c8"),
                        ExtendedMove::kingMoveMask | ExtendedMove::castlingMask,
                        0,
                        NOTATION::PIECES::KING | NOTATION::COLOR::BLACK, 0};
}
