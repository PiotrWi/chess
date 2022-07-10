# apt install python3.10-venv
# python3 -m venv .
# source bin/activate
# pip3 install chess


import asyncio
import chess
import chess.engine


command_white = "./UciApplication"
command_black = "./UciApplication"


async def play_single_game(command_white, command_black):
    t1, white_engine = await chess.engine.popen_uci(command_white)
    t2, black_engine = await chess.engine.popen_uci(command_black)

    board = chess.Board()
    print(board)
    print("")

    while not board.is_game_over():
        result = await white_engine.play(board, chess.engine.Limit(time=0.1))
        board.push(result.move)
        print(board)
        print("")
        if board.is_game_over():
            break
        result = await black_engine.play(board, chess.engine.Limit(time=0.1))
        board.push(result.move)
        print(board)
        print("")

    print(board.outcome().result())
    await white_engine.quit()
    await black_engine.quit()



asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
asyncio.run(play_single_game(command_white, command_black))

