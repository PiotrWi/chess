#pragma once

#include <vector>

#include <publicIf/Board.hpp>

namespace MoveGenerator
{

struct NotCheckedTag {};

/*
* Name is temporary until it is proven the nev version has better performance
*/
class MoveGeneratorV2
{
public:
    explicit MoveGeneratorV2(const Board& board, NOTATION::COLOR::color playerToGenerate);
    unsigned getValidMoveCount();
    unsigned getValidMoveCount(NotCheckedTag);
    std::vector<ExtendedMove> generateBeatingMoves();
    std::vector<ExtendedMove> generateNonBeatingMoves();
    std::vector<ExtendedMove> generateMoves();

private:
    void initialize();
    void calculateMoveTables();
    void calculateMoveCount();
    void evaluateKnights(uint64_t knightsBitMask, uint64_t forbidenFields);
    void evaluateKing(uint64_t forbidenFields);
    void evaluatePawns(uint64_t pawnsBitMask, uint64_t forbidenFields);
private:
    Board board;

    NOTATION::COLOR::color pieceColor;
    unsigned char kingPosition;
    unsigned char kingAttackersCount;
    uint64_t possibleBlockersMask;

    unsigned int moveCount;

    enum class State
    {
        Uninitialized,
        MoveTablesCalculated,
        BeatingMovesCalculated,
        AllMovesCalculated,
    } state;

    struct MoveTable
    {
        enum class Type
        {
            KnightMoves,
            KingMoves,
            SinglePawnMoves,
            DoublePawnMoves,
            PawnBeatingsRight,
            PawnBeatingsLeft,
        } type;
        unsigned char sourceField;
        uint64_t bitField; 
    };
    std::vector<MoveTable> moveTables;
    std::vector<ExtendedMove> moves;
};

}  // namespace MoveGenerator