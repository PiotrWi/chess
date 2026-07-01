# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

A personal chess engine project (C++20, Linux/gcc). It contains a UCI-compatible engine (usable from xboard-style
GUIs), a console application for playing locally, and the supporting board/search/evaluation libraries. The project
is explicitly a playground/work-in-progress — some functionality may be broken at any given time.

## Build & test commands

Build is CMake-based, wrapped by a `Makefile`. Everything configures into `./build` and binaries land in `./bin`.

```
make applications      # build ConsoleApplication + UciApplication into bin/
make ut                # build the unit test binary (bin/ut)
make ut-run             # build + run unit tests via ctest (test name: ut-run)
make pt                # build the performance/benchmark test binary (bin/pt)
make pt-run             # build + run performance tests via ctest (test name: pt-run)
make clean              # remove build/ and bin/
```

Debug builds (asserts on, `-Og -g`) via `CMAKE_ARGS`, e.g.:
```
make ut CMAKE_ARGS='-DCMAKE_BUILD_TYPE=Debug'
```
Default (non-Debug) builds use `-Ofast -flto -march=haswell` and define `USE_PEXT` (relies on BMI2 `pext`; the CPU
running tests must support it, or built binaries must run on such hardware).

### Running a single test

Tests are gtest-based; `ut` and `pt` are plain gtest binaries, so filter directly instead of going through ctest:
```
./bin/ut --gtest_filter='MoveGeneratorV2Test.*'
./bin/ut --gtest_filter='*SomeSpecificTest*'
```
`ctest --test-dir build -R ut-run` runs the whole binary as one ctest case (no gtest filtering).

### Dependencies

- Boost (headers only, `find_package(Boost)`) — used for `boost::container::small_vector`.
- GTest and Google Benchmark are expected to be discoverable via `find_package` (installed system-wide); despite
  `externals/gtest`/`externals/benchmark` being referenced in the top-level `CMakeLists.txt`, they are not vendored
  in this repo.
- `externals/non_std/non_std` is a **git submodule** (https://github.com/PiotrWi/non_std). Run
  `git submodule update --init --recursive` before building if it's empty.
- CI (`.github/workflows/c-cpp.yml`) runs on `ubuntu-latest` with `gcc-10`/`g++-10`, installs `libboost1.71-dev`,
  then runs `make all`, `make ut-run`, `make pt-run`.

### Other scripts

- `make strenght_comparator-run` / `make coefficient_adjuster-run` — run Python tooling under
  `applications/python/` (`StrenghtComparator.py`, `CoefficientAdjuster.py`) that plays the built UCI engine against
  Stockfish (via `python-chess`) to compare strength / tune evaluator coefficients. They require `applications`
  to be built first and read binaries via the `CHESS_BIN_DIR` env var (see `applications/python/lib/GameUtils.py`
  for the engine registry, e.g. `avaiableEngines["UciApplication"]`, `Stockfish1900`, etc.).

## Architecture

The codebase is split into independent CMake libraries under the top-level dirs, wired together in
`CMakeLists.txt`:

- **BoardLibrary** — board representation, move generation/application/validation, notation parsing. No search or
  evaluation logic.
- **SearchLibrary** — alpha-beta search on top of `BoardLibrary`, plus move ordering/caching. Depends on an
  evaluator loaded dynamically at runtime (see below), not linked at compile time.
- **Evaluators** — position evaluators, built as *separate shared libraries* (`StandardEvaluator`,
  `CustomizableEvaluator`), loaded via `dlopen`/`dlsym` at runtime rather than linked directly.
- **applications** — `ConsoleApplication` (local play) and `UciApplication` (UCI protocol engine), both linking
  `BoardLibrary` + `SearchLibrary` and pointed at an evaluator `.so` path at startup.

### Board representation (`BoardLibrary/core`)

- `Board` (`core/Board.hpp`) holds two `PlayerBitMasksSet` (one per color), each a set of `uint64_t` bitboards
  (pawns/rocks/knights/bishops/queens/kings), plus castling rights, en-passant file, and side to move.
- Pieces are encoded as 4-bit values in `NOTATION::PIECES` (`Notation.hpp`); the low bit of `NOTATION::COLOR` is
  ORed in for square-content lookups (`Board::getField`).
- `Move` is a plain from/to/promotion move; `ExtendedMove` additionally carries flag bits (capture, castling,
  en-passant, pawn double-push, king/rock move, promotion) precomputed by the move generator so downstream code
  (apply/undo, search, ordering) doesn't need to re-derive them from the board.
- `BoardEngine` (`BoardLibrary/BoardEngine.hpp`) is the main façade used by search/applications: it owns a `Board`,
  the incremental `Zobrist` hash, and the `ResultEvaluator`, and exposes `applyMove`/`undoMove` (with
  `SimpleMoveMemorial` for undo), `getMoveGeneratorV2`, `getResult`, and repetition checks.

### Move generation (`BoardLibrary/MoveGeneratorV2`)

`MoveGeneratorV2` is bitboard-based, computing pins/checks and pseudo-legal move sets per piece type in one pass
(`calculateMoveTables`), then materializing them into `boost::container::small_vector`-backed beating/non-beating
move lists on demand (`generateBeatingMoves`/`generateNonBeatingMoves`). It relies on precomputed lookup tables
(`core/bitboards/bitboardslookups.*`) and, when `USE_PEXT` is defined, BMI2 `pext`-based sliding-piece attacks
(`core/bitboards/PextBitBoards.cpp`).

### Search (`SearchLibrary`)

`FullSearchingImplementation.hpp` implements alpha-beta search with a quiescence search
(`quiescenceSearch`/`evaluateMax`), driven by `BoardEngine` for move application and `MoveGeneratorV2` for move
generation. Moves are ordered via `MoveOrdering.hpp`/`MVVLVA.hpp`, and quiescence capture ordering uses SEE
(`BoardLibrary/PositionAnalyzers/see.cpp`) through `OnlyBeatingMovesSeeVersion`. `Constants.hpp` holds shared
tunables (`MAX_DEPTH`, piece values). `CachedEngines/FullCachedEngine` wraps the evaluator plugin (loads the `.so`
via `dlopen`, resolves `init`/`evaluatePosition` via `dlsym`) and presumably caches evaluation/search results keyed
by position hash.

### Evaluator plugin ABI (`Evaluators/evaluatorIf.hpp`)

Evaluators are `extern "C"` shared libraries implementing `init(configFile)` and
`evaluatePosition(BoardEngine&, validMovesCount)` (plus an AVX variant). `StandardEvaluator` combines material,
piece-square tables, pawn structure, and king safety (`Evaluators/Common/*.hpp`) into per-side feature/coefficient
arrays. `CustomizableEvaluator` loads coefficients from an XML config
(`CustomizableEvaluatorConfig.xml`) instead of hardcoding them — this is what the Python coefficient-tuning script
drives. Applications choose the evaluator `.so` and config path via CLI flags (`--customEvaluator`,
`--evaluatorConfig`) or UCI `setoption`.

### UCI application (`applications/UciApplication`)

Runs an event-driven loop: `CommandParser` parses stdin UCI text into typed command structs (`Events.hpp`),
`EventsPropagator` dispatches them to handlers registered via `registerToEvent` (some run on a separate thread
started in `main`), and `GameHandler` owns the `BoardEngine` + `FullCachedEngine` pair driving actual play
(`onPositionProc`, `onGo`).

### Console application (`applications/ConsoleApplication`)

`IPlayer` is the interface for a side; `HumanPlayer` reads moves from stdin, `CachedPlayer`
(`ComputerPlayer/CachedPlayer.*`) wraps the search engine using a given evaluator `.so`. `main.cpp` parses
`--whitePlayer`/`--blackPlayer` (`human`/computer type), `--customEvaluator`, `--evaluatorConfig` CLI args.

## Tests (`test/`)

- `test/ut` — GTest unit tests, one file roughly per component (`MoveGeneratorV2Test`, `MoveApplierTest`,
  `MoveValidatorTest`, `CheckCheckerTest`, `ZobrishTest`, `uciNotationTest`, `CommandParserTest`, `seeTest`, etc.),
  plus `mt/WholeGame{1,2,3}.cpp` — full-game regression tests replaying entire games move by move. Shared board
  setup helpers live in `test/utils/BoardGenerationUtils.*`.
- `test/performanceTests` — Google Benchmark-based perf tests (move generation, search stability); results tracked
  informally in `BenchTimes.txt`.
- When adding a new test file, register it explicitly in `test/ut/CMakeLists.txt` (or
  `test/performanceTests/CMakeLists.txt`) — sources are listed individually, not globbed.
