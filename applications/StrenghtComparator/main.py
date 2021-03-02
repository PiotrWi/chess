import asyncio
import sys
import chess
import chess.engine
import logging
import time

testedEngineCommand = sys.argv[1]
referenceEngineCommand = sys.argv[2]

GAME_TIME=180.0
ADDITIONAL_TIME_PER_MOVE=5.0

def main():
    if len(sys.argv) == 1:
        print("""Syntax is: ./main.py "comand to run engine to be tested" "comand to run refernece engine" REF_ENGINE_RANKING """)
        print("""Ex: ./main.py "/home/uciEngine" "/home/uciEngine_2" 1000""")
        exit(1)
    print(sys.argv)

    move = 0
    engines = [chess.engine.SimpleEngine.popen_uci(sys.argv[1]), chess.engine.SimpleEngine.popen_uci(sys.argv[2])]
    clocks = [GAME_TIME, GAME_TIME]

    board = chess.Board()
    while not board.is_game_over():
        a = time.monotonic_ns()
        result = engines[move % 2].play(board, chess.engine.Limit(white_clock = clocks[0], black_clock = clocks[1]))
        b = time.monotonic_ns()
        clocks[move % 2] -= (b-a)/1000000000 + ADDITIONAL_TIME_PER_MOVE
        board.push(result.move)
        move = move + 1

    engines[0].quit()
    engines[1].quit()


logging.basicConfig(level=logging.DEBUG)

if __name__ == "__main__":
     main()

