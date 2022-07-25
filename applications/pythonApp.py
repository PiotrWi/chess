import asyncio
import chess
import chess.engine
import time

command_white = "./UciApplication"
options_white = {"customEvaluator": "libCustomizableEvaluator.so", "evaluatorConfig": "CustomizableEvaluatorConfig.xml"}
command_black = "./UciApplication"
options_black = {"customEvaluator": "libCustomizableEvaluator.so", "evaluatorConfig": "CustomizableEvaluatorConfig.xml"}


def ns_to_s(nanoseconds):
    return nanoseconds / 1000000000.0


class Clock:
    def __init__(self, remaining_time, time_per_move):
        self.remaining_time = remaining_time
        self.time_per_move = time_per_move
        self.timepoint_start = 0

    def start_ticking(self):
        self.timepoint_start = time.monotonic_ns()

    def stop_ticking(self):
        timepoint_end = time.monotonic_ns()
        self.remaining_time = self.remaining_time - ns_to_s(timepoint_end - self.timepoint_start)
        if not self.is_expired():
            self.remaining_time = self.time_per_move

    def is_expired(self):
        return self.remaining_time <= 0.0

    def get(self):
        return self.remaining_time


class SingleGameHandler:
    def __init__(self, white_command, white_options, black_command, black_options):
        self.white_command = white_command
        self.white_options = white_options
        self.black_command = black_command
        self.black_options = black_options
        self.white_engine = chess.engine.UciProtocol()
        self.black_engine = chess.engine.UciProtocol()
        self.board = chess.Board()
        self.white_clock = Clock(60.0, 1.0)
        self.black_clock = Clock(60.0, 1.0)

    async def _open_engines(self):
        t1, self.white_engine = await chess.engine.popen_uci(self.white_command)
        t2, self.black_engine = await chess.engine.popen_uci(self.black_command)

    async def _close_engines(self):
        await self.white_engine.quit()
        await self.black_engine.quit()

    async def _configure_engines(self):
        await self.white_engine.configure(self.white_options)
        await self.black_engine.configure(self.black_options)

    async def _play_single_move(self, engine, clock):
        clock.start_ticking()
        result = await engine.play(self.board,
                                   chess.engine.Limit(white_clock=self.white_clock.get(),
                                                      black_clock=self.black_clock.get()))
        clock.stop_ticking()

        print(result.move)
        self.board.push(result.move)
        print("{}\n".format(self.board))

    def _is_game_over(self):
        return self.white_clock.is_expired() or self.black_clock.is_expired() or self.board.is_game_over()

    def get_result(self):
        if self.white_clock.is_expired():
            return "0-1"
        if self.black_clock.is_expired():
            return "1-0"
        return self.board.outcome()

    async def play_single_game(self):
        self.board = chess.Board()
        print("{}\n".format(self.board))

        try:
            await self._open_engines()
            await self._configure_engines()

            while not self._is_game_over():
                await self._play_single_move(self.white_engine, self.white_clock)
                if self._is_game_over():
                    break
                await self._play_single_move(self.black_engine, self.black_clock)
        except ZeroDivisionError as err:
            print('Exception occurred:', err)

        await self._close_engines()
        print(self.get_result())
        return self.get_result()


async def play_single_game():
    game = SingleGameHandler(command_white, options_white, command_black, options_black)
    await game.play_single_game()


asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
asyncio.run(play_single_game())
