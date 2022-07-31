#pragma once

struct SuareTableCoeffictients
{
    int white_pawn[64];
    int black_pawn[64];
    int white_knight[64];
    int black_knight[64];
    int white_king[64];
    int black_king[64];
    int white_bishop[64];
    int black_bishop[64];
    int white_rock[64];
    int black_rock[64];
    int white_queen[64];
    int black_queen[64];
};

unsigned char reverseLookup[64] ={ 
	56, 57, 58, 59, 60, 61, 61, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7
};

void reverseTable(int sourceTable[], int targetTable[])
{
	for (auto i = 0; i < 64; ++i)
	{
		targetTable[i] = sourceTable[reverseLookup[i]];
	}
}

namespace square_tables_evaluator
{

int getOccupiedBitIndexAndClearIt(uint64_t& bitMask)
{
    auto index = 63 - __builtin_clzll(bitMask);
    auto bitMaskOfIndex = 1ull << index;
    bitMask ^= bitMaskOfIndex;
    return index;
}

int evalSingle(const int coeffincients[64], uint64_t bitMask)
{
    int eval = 0;
    while (bitMask)
    {
        auto index = getOccupiedBitIndexAndClearIt(bitMask);
        eval += coeffincients[index];
    }
    return eval;
}

int evaluate(const Board& board, NOTATION::COLOR::color playerOnMove, const SuareTableCoeffictients& coeffincients)
{
    int eval = 0;
    eval += evalSingle(coeffincients.white_pawn, board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask);
    eval -= evalSingle(coeffincients.black_pawn, board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask);
    eval += evalSingle(coeffincients.white_knight, board.piecesBitSets[NOTATION::COLOR::WHITE].knightsMask);
    eval -= evalSingle(coeffincients.black_knight, board.piecesBitSets[NOTATION::COLOR::BLACK].knightsMask);
    eval += evalSingle(coeffincients.white_king, board.piecesBitSets[NOTATION::COLOR::WHITE].kingsMask);
    eval -= evalSingle(coeffincients.black_king, board.piecesBitSets[NOTATION::COLOR::BLACK].kingsMask);
    eval += evalSingle(coeffincients.white_bishop, board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask);
    eval -= evalSingle(coeffincients.black_bishop, board.piecesBitSets[NOTATION::COLOR::BLACK].bishopsMask);
    eval += evalSingle(coeffincients.white_rock, board.piecesBitSets[NOTATION::COLOR::WHITE].rocksMask);
    eval -= evalSingle(coeffincients.black_rock, board.piecesBitSets[NOTATION::COLOR::BLACK].rocksMask);
    eval += evalSingle(coeffincients.white_queen, board.piecesBitSets[NOTATION::COLOR::WHITE].queensMask);
    eval -= evalSingle(coeffincients.black_queen, board.piecesBitSets[NOTATION::COLOR::BLACK].queensMask);

    return (playerOnMove == NOTATION::COLOR::color::white)
        ? eval
        : -1 * eval;
}

}  // namespace square_tables_evaluator
