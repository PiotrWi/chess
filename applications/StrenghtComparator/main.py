import asyncio
import sys
import chess
import chess.engine
import logging
import time

testedEngineCommand = sys.argv[1]
referenceEngineCommand = sys.argv[2]

GAME_TIME=60.0
ADDITIONAL_TIME_PER_MOVE=1.0


class Game(object):
    def __init__(self, white_player, white_command, black_player, black_command, fen):
        self.whitePlayer = white_player
        self.whiteCommand = white_command
        self.blackPlayer = black_player
        self.blackCommand = black_command
        self.fen = fen
        self.result = ""

fenList = ["rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r1bqkb1r/1ppp1ppp/p1n2n2/4p3/B3P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1",
        "rnbqkb1r/ppp2ppp/1n6/4p3/8/2N3P1/PP1PPPBP/R1BQK1NR w KQkq - 2 6",
        "rnbqkb1r/ppp2ppp/1n6/4p3/8/2N3P1/PP1PPPBP/R1BQK1NR w KQkq - 2 6"]
enqueuedGames =[]
playedGames = []


async def playSingleGame():
    try:
        while len(enqueuedGames) != 0:
            game = enqueuedGames.pop(0)
            move = 0
            engines = [(await chess.engine.popen_uci(game.whiteCommand))[1],
                       (await chess.engine.popen_uci(game.blackCommand))[1]]

            clocks = [GAME_TIME, GAME_TIME]

            claimDraw = False
            try:
                board = chess.Board(game.fen)
                while not board.is_game_over() or board.is_repetition(3) or 100 == board.halfmove_clock(): #claim_draw true means that draw rules are: 3 repeatitions, 50 repeatitions. Since 2014 official rules are different.
                    a = time.monotonic_ns()
                    result = await engines[move % 2].play(board, chess.engine.Limit(white_clock=clocks[0], black_clock=clocks[1]))
                    b = time.monotonic_ns()
                    clocks[move % 2] = clocks[move % 2] - (b-a)/1000000000 + ADDITIONAL_TIME_PER_MOVE
                    if not result.move in board.legal_moves:
                        print("Invalid move: " + str(presult.move))
                        raise ValueError
                    board.push(result.move)
                    move = move + 1

                await engines[0].quit()
                await engines[1].quit()
            except:
                print("Tearing down engines due to exception")
                print(board)
                await engines[0].quit()
                await engines[1].quit()
            if board.is_repetition(3) or 100 == board.halfmove_clock():
                game.result = "1/2 - 1/2"
            else:
                game.result = board.result()
            playedGames.append(game)
    except:
        print("Undefined error")


async def main():
    if len(sys.argv) == 1:
        print("""Syntax is: ./main.py "comand to run engine to be tested" "comand to run refernece engine" REF_ENGINE_RANKING """)
        print("""Ex: ./main.py "/home/uciEngine" "/home/uciEngine_2" 1000""")
        exit(1)

    for fen in fenList:
        enqueuedGames.append(Game(white_player="tested",
                              white_command=testedEngineCommand,
                              black_player="referenced",
                              black_command=referenceEngineCommand,
                              fen=fen))
        enqueuedGames.append(Game(white_player="referenced",
                              white_command=referenceEngineCommand,
                              black_player="tested",
                              black_command=testedEngineCommand,
                              fen=fen))

    task1 = asyncio.create_task(playSingleGame())
    task2 = asyncio.create_task(playSingleGame())
    task3 = asyncio.create_task(playSingleGame())
    await task1
    await task2
    await task3
    for gameResult in playedGames:
        print (gameResult.whitePlayer + " " + gameResult.blackPlayer + ": " + gameResult.result)

logging.basicConfig(level=logging.DEBUG)


if __name__ == "__main__":
    asyncio.run(main())
