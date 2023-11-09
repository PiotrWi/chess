from lib.GameUtils import *
asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
s = StrengthComarator(3, avaiableEngines["UciApplicationMl"], avaiableEngines["UciApplicationMl"])
asyncio.run(s.start_play(150))
