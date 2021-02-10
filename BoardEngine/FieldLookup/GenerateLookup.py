colors = ["white", "black"]

BaseTemplate = """// File is generated by FieldLookup.py
// In order to change it please modify FieldLookup.py
// run it
// and overlap FieldLookup.hpp by tmp_FieldLookup.hpp

#pragma once

#include <publicIf/Notation.hpp>

template <NOTATION::COLOR::color color, unsigned char kingPosition>
struct FieldLookup
{
    static constexpr unsigned char OppositePawnPositionsSize = 0u;
    static constexpr unsigned char OppositePawnPositions[1] = {};

    static constexpr unsigned char KingPossibleMovesSize = 0u;
    static constexpr unsigned char KingPossibleMoves[1] = {};
    
    static constexpr unsigned char KnightPossibleMovesSize = 0u;
    static constexpr unsigned char KnightPossibleMoves[1] = {};

    static constexpr unsigned char TopPossibleMovesSize = 0u;
    static constexpr unsigned char TopPossibleMoves[1] = {};

    static constexpr unsigned char BottomPossibleMovesSize = 0u;
    static constexpr unsigned char BottomPossibleMoves[1] = {};

    static constexpr unsigned char LeftPossibleMovesSize = 0u;
    static constexpr unsigned char LeftPossibleMoves[1] = {};

    static constexpr unsigned char RightPossibleMovesSize = 0u;
    static constexpr unsigned char RightPossibleMoves[1] = {};

    static constexpr unsigned char TopLeftPossibleMovesSize = 0u;
    static constexpr unsigned char TopLeftPossibleMoves[1] = {};

    static constexpr unsigned char TopRightPossibleMovesSize = 0u;
    static constexpr unsigned char TopRightPossibleMoves[1] = {};
    
    static constexpr unsigned char BottomLeftPossibleMovesSize = 0u;
    static constexpr unsigned char BottomLeftPossibleMoves[1] = {};

    static constexpr unsigned char BottomRightPossibleMovesSize = 0u;
    static constexpr unsigned char BottomRightPossibleMoves[1] = {};

    static constexpr bool CanPawnMove2Fields = false;
    static constexpr bool CanPawnPromote = false;
    static constexpr unsigned char PawnAttackCandidatesSize = 0;
    static constexpr unsigned char PawnAttackCandidates[1] = {};
};"""


def get_row_col(pos):
    return pos // 8, pos % 8


def get_position(row_col):
    return row_col[0] * 8 + row_col[1]


def is_valid_pos(row_col):
    return row_col[0] >= 0 and row_col[0] < 8 and row_col[1] >= 0 and row_col[1] < 8


def generateOpositePawnsPositions(c, pos):
    positions = []

    king_pos = get_row_col(pos)

    if c == "white":
        if pos >= 48:
            return []
        left_pawn = [king_pos[0]+1, king_pos[1]-1]
        if is_valid_pos(left_pawn):
            positions.append(get_position(left_pawn))
        right_pawn = [king_pos[0]+1, king_pos[1]+1]
        if is_valid_pos(right_pawn):
            positions.append(get_position(right_pawn))
    if c == "black":
        if pos < 16:
            return []
        left_pawn = [king_pos[0]-1, king_pos[1]-1]
        if is_valid_pos(left_pawn):
            positions.append(get_position(left_pawn))
        right_pawn = [king_pos[0]-1, king_pos[1]+1]
        if is_valid_pos(right_pawn):
            positions.append(get_position(right_pawn))

    return positions


def generateKingPositions(pos):
    positions = []

    for positionDiff in [ [1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0], [-1, -1], [0, -1], [1, -1] ]:
        row_col = get_row_col(pos)
        king_pos = [row_col[0] + positionDiff[0], row_col[1] + positionDiff[1]]
        if is_valid_pos(king_pos):
            positions.append(get_position(king_pos))
    return positions


def generateKnightPositions(pos):
    positions = []

    for positionDiff in [ [1, -2], [2, -1], [2, 1], [1, 2], [-1, 2], [-2, 1], [-2, -1], [-1, -2] ]:
        row_col = get_row_col(pos)
        knight_pos = [row_col[0] + positionDiff[0], row_col[1] + positionDiff[1]]
        if is_valid_pos(knight_pos):
            positions.append(get_position(knight_pos))
    return positions


def generateLines(pos):
    positions = []
    for positionDiff in [["BottomLeft", -1, -1], ["BottomRight", -1, 1], ["TopLeft", 1, -1], ["TopRight", 1, 1], ["Bottom", -1, 0], ["Top", 1, 0], ["Left", 0, -1], ["Right", 0, 1]]:
        positions.append([positionDiff[0], []])
        row_col = get_row_col(pos)
        eval_pos = [row_col[0] + positionDiff[1], row_col[1] + positionDiff[2]]
        while is_valid_pos(eval_pos):
            positions[-1][1].append(get_position(eval_pos))
            eval_pos = [eval_pos[0] + positionDiff[1], eval_pos[1] + positionDiff[2]]
    return positions


def can_pawn_move_2_fields(c, pos):
    if c == "white" and pos >= 8 and pos < 16:
        return "true"
    if c == "black" and pos >= 48 and pos < 56:
        return "true"
    return "false"


def can_pawn_promote(c, pos):
    if c == "white" and pos >= 48 and pos < 56:
        return "true"
    if c == "black" and pos >= 8 and pos < 16:
        return "true"
    return "false"


def generatePawnsBeatings(c, pos):
    positions = []

    king_pos = get_row_col(pos)

    if c == "white":
        if 8 <= pos < 56:
            left_pawn = [king_pos[0]+1, king_pos[1]-1]
            if is_valid_pos(left_pawn):
                positions.append(get_position(left_pawn))
            right_pawn = [king_pos[0]+1, king_pos[1]+1]
            if is_valid_pos(right_pawn):
                positions.append(get_position(right_pawn))
    if c == "black":
        if 8 <= pos < 56:
            left_pawn = [king_pos[0]-1, king_pos[1]-1]
            if is_valid_pos(left_pawn):
                positions.append(get_position(left_pawn))
            right_pawn = [king_pos[0]-1, king_pos[1]+1]
            if is_valid_pos(right_pawn):
                positions.append(get_position(right_pawn))

    return positions


def writeSingleLine(f, prefix, list):
    f.write("    static constexpr unsigned char {0}PossibleMovesSize = {1};\n".format(prefix, str(len(list))))
    f.write("    static constexpr unsigned char {0}PossibleMoves[{1}] = ".format(prefix, str(max(1, len(list)))) + "{" + "{0}".format(str(list)[1:-1]) + "};\n")


with open("tmp_FieldLookup.hpp", "w") as f:
    f.write(BaseTemplate)
    for c in colors:
        for position in range(0, 64):
            f.write("template <>\n")
            f.write("struct FieldLookup<NOTATION::COLOR::color::{0}, {1}>".format(c, str(position)) + "{\n")

            valid_opposite_pawns_positions = generateOpositePawnsPositions(c, position)
            f.write("    static constexpr unsigned char OppositePawnPositionsSize = {0};\n".format(str(len(valid_opposite_pawns_positions))))
            f.write("    static constexpr unsigned char OppositePawnPositions[{0}] = ".format(str(max(1, len(valid_opposite_pawns_positions)))) + "{" + "{0}".format(str(valid_opposite_pawns_positions)[1:-1]) + "};\n")

            valid_king_positions = generateKingPositions(position)
            writeSingleLine(f, "King", valid_king_positions)

            valid_knight_positions = generateKnightPositions(position)
            writeSingleLine(f, "Knight", valid_knight_positions)

            lines = generateLines(position)
            for line in lines:
                writeSingleLine(f, line[0], line[1])

            f.write("    static constexpr bool CanPawnMove2Fields = {0};\n".format(can_pawn_move_2_fields(c, position)))
            f.write("    static constexpr bool CanPawnPromote = {0};\n".format(can_pawn_promote(c, position)))

            pawns_beatings = generatePawnsBeatings(c, position)
            f.write("    static constexpr unsigned char PawnAttackCandidatesSize = {0};\n".format(str(len(pawns_beatings))))
            f.write("    static constexpr unsigned char PawnAttackCandidates[{0}] = ".format(str(max(1, len(pawns_beatings)))) + "{" + "{0}".format(str(pawns_beatings)[1:-1]) + "};\n")

            f.write("};\n")