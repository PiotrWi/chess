from lib.GameUtils import *

asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
ac = AdjustCoefficients(avaiableEngines["UciWithCustomEvaluator"], avaiableEngines["Stockfish1700"])
ac.adjust()
