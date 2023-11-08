from lib.GameUtils import *
asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
s = StrengthComarator(1, avaiableEngines["UciApplicationMl"], avaiableEngines["Stockfish1900"])
asyncio.run(s.start_play(150))
