#pragma once

#include <iosfwd>
#include "Notation.hpp"

struct Board
{
    struct PlayerBitMasksSet
    {
        uint64_t pawnsMask = 0ull;
        uint64_t rocksMask = 0ull;
        uint64_t knightsMask = 0ull;
        uint64_t bishopsMask = 0ull;
        uint64_t queensMask = 0ull;
        uint64_t kingsMask = 0ull;
        bool operator==(const PlayerBitMasksSet&) const;
    };

    PlayerBitMasksSet piecesBitSets[2];
    uint8_t castlingRights = NOTATION::CASTLING_RIGHTS::CASTLING_MASK;
    signed char validEnPassant = -1;
    NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white;

    unsigned char getField(const unsigned char field) const noexcept;
    unsigned char getFieldForNonEmpty(const unsigned char field, NOTATION::COLOR::color c) const noexcept;
    unsigned char getField(const char* field) const noexcept;
    void setPreviouslyEmptyField(const unsigned char field, unsigned char value) noexcept;
    void setField(const unsigned char field, unsigned char value) noexcept;
    void setField(const unsigned char field, unsigned char oldValue, unsigned char value) noexcept;
    void setField(const char* field, unsigned char val) noexcept;
    void clearField(const unsigned char field, unsigned char oldValue) noexcept;

    bool isCorrect() const noexcept;
private:
    void cleanConcrete(const unsigned char field, unsigned char oldValue) noexcept;
    void setConcrete(const unsigned char field, unsigned char value) noexcept;

    constexpr static unsigned char WHITE_INDEX = 0;
    constexpr static unsigned char BLACK_INDEX = 1;
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

struct ExtendedMove
{
    constexpr static unsigned char promotionMask = 0b00000001;
    constexpr static unsigned char beatingMask   = 0b00000010;
    constexpr static unsigned char pawnMoveMask  = 0b00000100;
    constexpr static unsigned char kingMoveMask  = 0b00001000;
    constexpr static unsigned char enPasantMask  = 0b00010000;
    constexpr static unsigned char castlingMask  = 0b00100000;
    constexpr static unsigned char rockMoveMask  = 0b01000000;

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

    explicit operator Move() const;
    static ExtendedMove whiteShortCaste() noexcept;
    static ExtendedMove whiteLongCaste() noexcept;
    static ExtendedMove blackShortCaste() noexcept;
    static ExtendedMove blackLongCaste() noexcept;
};

bool operator==(const ExtendedMove& lfs, const ExtendedMove& rhs) noexcept;
bool operator==(const Move& lfs, const Move& rhs) noexcept;

// For non time relevant usages
ExtendedMove convertMoveToExtended(const Board&, const Move&) noexcept;
