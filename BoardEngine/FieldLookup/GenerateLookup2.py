colors = ["white", "black"]

BaseTemplate = """// File is generated by FieldLookup.py
// In order to change it please modify FieldLookup.py
// run it
// and overlap FieldLookup.hpp by tmp_FieldLookup.hpp

#pragma once
"""

#constexpr unsigned char knigthMoves[64] =
#{
#};


def is_valid_pos(row_col):
    return row_col[0] >= 0 and row_col[0] < 8 and row_col[1] >= 0 and row_col[1] < 8


def get_row_col(pos):
    return pos // 8, pos % 8


def get_position(row_col):
    return row_col[0] * 8 + row_col[1]



def generateKnightPositions(pos):
    positions = []

    for positionDiff in [ [1, -2], [2, -1], [2, 1], [1, 2], [-1, 2], [-2, 1], [-2, -1], [-1, -2] ]:
        row_col = get_row_col(pos)
        knight_pos = [row_col[0] + positionDiff[0], row_col[1] + positionDiff[1]]
        if is_valid_pos(knight_pos):
            positions.append(get_position(knight_pos))

    return positions


with open("tmp_FieldLookup_2.hpp", "w") as f:
    f.write("#pragma once\n\n")
    f.write("constexpr signed char knigthMoves[64][9] =\n{\n")
    for i in range(0, 64):
        knigthPositions = generateKnightPositions(i)
        f.write("\t{" + "{0}".format(str(len(knigthPositions))) + ", " + "{0}".format(str(knigthPositions + [-1] * (8 - len(knigthPositions)))[1:-1]) + "},\n")
    f.write("};\n")