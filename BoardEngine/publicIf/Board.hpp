#pragma once

#include <iosfwd>
#include <publicIf/Notation.hpp>

struct Board
{
    NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white;
    signed char validEnPassant = -1;
    unsigned char castlingRights = 15u;
    unsigned char fields[64] = {}; // "0 belongs to A1, 1 belongs to B1, 8 belongs to A2

    unsigned char& operator[](const char*) noexcept;
    unsigned char& operator[](const unsigned char) noexcept;
    const unsigned char& operator[](const char*) const noexcept;
    const unsigned char& operator[](const unsigned char) const noexcept;
};

bool operator==(const Board& lhs, const Board& rhs) noexcept;

void initDefault(Board& board) noexcept;
struct Move
{
	Move() noexcept;
	explicit Move(const char*, const char*, bool isPromoted = false, unsigned char promoteTo = 0) noexcept;
    explicit Move(unsigned char, unsigned char, bool isPromoted = false, unsigned char promoteTo = 0) noexcept;

	unsigned char source;
	unsigned char destination;
	bool isPromoted;
	unsigned char promoteTo;
};

bool operator==(const Move& lfs, const Move& rhs) noexcept;

struct ExtendedMove
{
    constexpr static unsigned char promotionMask = 0b00000001;
    constexpr static unsigned char beatingMask   = 0b00000010;
    constexpr static unsigned char pawnMoveMask  = 0b00000100;
    constexpr static unsigned char kingMoveMask  = 0b00001000;

    ExtendedMove() noexcept = default;
    ExtendedMove(unsigned char sourceIn,
                 unsigned char destinationIn,
                 unsigned char flagsIn,
                 unsigned char promotingIn,
                 unsigned char sourcePieceIn,
                 unsigned char targetPieceIn) noexcept;

    unsigned char source;
    unsigned char destination;
    unsigned char flags;
    unsigned char promoting;
    unsigned char sourcePiece;
    unsigned char targetPiece;

    operator Move() const;
    static ExtendedMove whiteShortCaste();
    static ExtendedMove whiteLongCaste();
    static ExtendedMove blackShortCaste();
    static ExtendedMove blackLongCaste();
};

bool operator ==(const ExtendedMove& lfs, const ExtendedMove& rhs) noexcept;

// For non time relevant usages
ExtendedMove convertMoveToExtended(const Board&, const Move&) noexcept;
