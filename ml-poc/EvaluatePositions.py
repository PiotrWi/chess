import asyncio
import chess
import chess.engine
import sqlite3
import os


def get_app_command():
    return os.environ["CHESS_BIN_DIR"] + "/UciApplication"


class PositionEvaluationWorker:
    def __init__(self, db_connection, db_cursor, shared_index, records_to_process):
        self.db_connection = db_connection
        self.db_cursor = db_cursor
        self.shared_index = shared_index
        self.records_to_process = records_to_process
        self.board = chess.Board()

    async def run(self):
        t1, engine = await chess.engine.popen_uci(get_app_command())

        while self.shared_index[0] < len(self.records_to_process):
            current_record = self.records_to_process[self.shared_index[0]]
            self.shared_index[0] = self.shared_index[0] + 1
            fen = current_record[0]
            self.board = chess.Board(fen)
            try:
                result = await engine.play(self.board, chess.engine.Limit(time=15.0), info=chess.engine.INFO_ALL)
                print ((result.info["score"].relative.score(), result.info["score"]))
                self.db_cursor.execute("""UPDATE positions set evaluation={} where position == "{}";""".format(result.info["score"].relative.score(), fen))
                self.db_connection.commit()
            except Exception as err:
                t1, engine = await chess.engine.popen_uci(get_app_command())
                print('Exception occurred for fen: {}, ex: {}'.format(fen, err))


def has_beating_move(board):
    for move in board.legal_moves:
        if board.piece_at(move.to_square) is not None:
            return True
    return False


def has_promotion_move(board):
    for move in board.legal_moves:
        if move.promotion is not None:
            return True
    return False


class PositionEvaluation:
    def fill_is_position_stable(self):
        connection = sqlite3.connect("positions.db")
        cursor = connection.cursor()
        not_evaluated_positions = cursor.execute("select * from positions where isStable is NULL;").fetchall()
        for record in not_evaluated_positions:
            fen = record[0]
            board = chess.Board(fen)
            is_stable = (not board.is_check()) and (not has_beating_move(board)) and not (has_promotion_move(board))
            cursor.execute("""UPDATE positions set isStable={} where position == "{}";""".format(is_stable, fen))
            connection.commit()


    async def evaluate_positions_with_no_scores(self):
        connection = sqlite3.connect("positions.db")
        cursor = connection.cursor()
        not_evaluated_positions = cursor.execute("select * from positions where evaluation is NULL;").fetchall()
        index_refetence = [0]

        workers = []
        tasks = []
        for i in range(3):
            workers.append(PositionEvaluationWorker(connection, cursor, index_refetence, not_evaluated_positions))
            tasks.append(asyncio.create_task(workers[i].run()))

        for i in range(3):
            await tasks[i]


asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
pe = PositionEvaluation()
pe.fill_is_position_stable()
asyncio.run(pe.evaluate_positions_with_no_scores())
