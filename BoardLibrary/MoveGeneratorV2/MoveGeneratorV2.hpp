#pragma once

#include <span>
#include <boost/container/small_vector.hpp>
#include <publicIf/Board.hpp>

namespace MoveGenerator
{

struct NotCheckedTag {};

using TBeatingVector = boost::container::small_vector<ExtendedMove, 32>;
using TNormalVector = boost::container::small_vector<ExtendedMove, 200>;

/*
* Name is temporary until it is proven the nev version has better performance
*/
class MoveGeneratorV2
{
public:
    explicit MoveGeneratorV2(const Board& board, const NOTATION::COLOR::color playerToGenerate);
    MoveGeneratorV2(const MoveGeneratorV2&) = delete;
    MoveGeneratorV2& operator=(const MoveGeneratorV2&) = delete;
    MoveGeneratorV2(MoveGeneratorV2&&) = default;
    MoveGeneratorV2& operator=(MoveGeneratorV2&&) = default;
    unsigned getValidMoveCount();
    unsigned getValidMoveCount(NotCheckedTag);
    std::span<ExtendedMove> generateBeatingMoves();
    std::span<ExtendedMove> generateNonBeatingMoves();

private:
    void initialize();
    void calculateMoveTables();
    void calculateMoveCount();
    void evaluateKnights(uint64_t knightsBitMask, const uint64_t forbidenFields);
    void evaluateKing(const uint64_t forbidenFields);
    void evaluatePawns(uint64_t pawnsBitMask, const uint64_t opponentPieces);
    void evaluatePawns(uint64_t pawnsBitMask, const uint64_t opponentPieces, const uint64_t checkBlockers);
    void evaluatePawnsBeatings(uint64_t pawnsToMoveToRightTop, uint64_t pawnsToMoveToLeftTop, uint64_t opponentPieces);
    void evaluateEnPassant(uint64_t pawnsToMoveToRightTop, uint64_t pawnsToMoveToLeftTop);
    void evaluateRocks(uint64_t rocksBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields);
    void evaluatePinnedRocks(uint64_t rocksBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields);
    void evaluateBishops(uint64_t rocksBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields);
    void evaluatePinnedBishops(uint64_t rocksBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields);
    void evaluateQueen(uint64_t queensMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields);
    void evaluatePinnedQueen(uint64_t queensMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields);
    void evaluateCasles(const uint64_t allOccupiedFields);

private:
    Board board;

    const NOTATION::COLOR::color pieceColor;
    unsigned char kingPosition;
    unsigned char kingAttackersCount;
    uint64_t possibleBlockersMask;

    unsigned int moveCount;

    struct MoveTable
    {
        enum class Type : unsigned char
        {
            KnightMoves = NOTATION::PIECES::KNIGHT,
            KingMoves = 0b10000,
            SinglePawnMoves = 0b10001,
            DoublePawnMoves = 0b10010,
            PawnBeatingsRight = 0b10011,
            PawnBeatingsLeft =  0b10100,
            RockMoves = NOTATION::PIECES::ROCK,
            BishopMoves = NOTATION::PIECES::BISHOP,
            QueenMoves = NOTATION::PIECES::QUEEN,
            Castle = 0b10101,
        } type;
        unsigned char sourceField;
        uint64_t bitField; 
    };
    unsigned int moveTablesN = 0;
    MoveTable moveTables[21];

    TBeatingVector beatingMoves;
    TNormalVector normalMoves;
};

}  // namespace MoveGenerator