import sqlite3
import numpy as np
import pandas as pd
import chess
import chess.engine


table_colums = ['position', 'version', 'evaluation', 'isStable']


def get_all_evaluated_records():
    connection = sqlite3.connect("positions.db")
    cursor = connection.cursor()
    evaluated_positions = cursor.execute("""select * from positions where evaluation is not NULL;""").fetchall()
    dataset = pd.DataFrame(evaluated_positions, columns=table_colums)
    return dataset['position'].to_numpy(), dataset['evaluation'].to_numpy()


def is_white_pawn(piece):
    if piece is None:
        return False
    return piece.color == chess.WHITE and piece.piece_type == chess.PAWN


def is_black_pawn(piece):
    if piece is None:
        return False
    return piece.color == chess.BLACK and piece.piece_type == chess.PAWN


def is_white_rook(piece):
    if piece is None:
        return False
    return piece.color == chess.WHITE and piece.piece_type == chess.ROOK


def is_black_rook(piece):
    if piece is None:
        return False
    return piece.color == chess.BLACK and piece.piece_type == chess.ROOK


def is_white_knight(piece):
    if piece is None:
        return False
    return piece.color == chess.WHITE and piece.piece_type == chess.KNIGHT


def is_black_knight(piece):
    if piece is None:
        return False
    return piece.color == chess.BLACK and piece.piece_type == chess.KNIGHT


def is_white_bishop(piece):
    if piece is None:
        return False
    return piece.color == chess.WHITE and piece.piece_type == chess.BISHOP


def is_black_bishop(piece):
    if piece is None:
        return False
    return piece.color == chess.BLACK and piece.piece_type == chess.BISHOP


def is_white_queen(piece):
    if piece is None:
        return False
    return piece.color == chess.WHITE and piece.piece_type == chess.QUEEN


def is_black_queen(piece):
    if piece is None:
        return False
    return piece.color == chess.BLACK and piece.piece_type == chess.QUEEN


def is_white_king(piece):
    if piece is None:
        return False
    return piece.color == chess.WHITE and piece.piece_type == chess.KING


def is_black_king(piece):
    if piece is None:
        return False
    return piece.color == chess.BLACK and piece.piece_type == chess.KING



def fen_positon_to_one_hot_vector(fen):
    board = chess.Board(fen)
    return ([1 if board.turn == chess.WHITE else -1]
            + [board.has_kingside_castling_rights(chess.WHITE), board.has_kingside_castling_rights(chess.BLACK)]
            + [board.has_queenside_castling_rights(chess.WHITE), board.has_queenside_castling_rights(chess.BLACK)]
            + [board.ep_square == field for field in range(64)]
            + [is_white_pawn(board.piece_at(field)) for field in range(64)]
            + [is_black_pawn(board.piece_at(field)) for field in range(64)]
            + [is_white_rook(board.piece_at(field)) for field in range(64)]
            + [is_black_rook(board.piece_at(field)) for field in range(64)]
            + [is_white_knight(board.piece_at(field)) for field in range(64)]
            + [is_black_knight(board.piece_at(field)) for field in range(64)]
            + [is_white_bishop(board.piece_at(field)) for field in range(64)]
            + [is_black_bishop(board.piece_at(field)) for field in range(64)]
            + [is_white_queen(board.piece_at(field)) for field in range(64)]
            + [is_black_queen(board.piece_at(field)) for field in range(64)]
            + [is_white_king(board.piece_at(field)) for field in range(64)]
            + [is_black_king(board.piece_at(field)) for field in range(64)])


position, evaluation = get_all_evaluated_records()
