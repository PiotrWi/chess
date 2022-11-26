from lib.GameUtils import *

asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
s = StrengthComarator(12, avaiableEngines["UciApplication"], avaiableEngines["Stockfish1900"])
asyncio.run(s.start_play(150))
