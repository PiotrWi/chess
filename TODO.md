# Engine review findings / TODO

Findings from a manual code review (2026-07-01). Fixed items have been removed from this list;
see git log for their commits. Remaining items below are either still open or intentionally
deferred/won't-fix, with the reasoning kept so it isn't re-litigated later.

## Correctness bugs

- [won't fix] **Threefold-repetition check ignores castling rights and en passant.** `BoardLibrary/ResultEvaluator/ResultEvaluator.cpp:28-30`.
      `compare()` only compares `piecesBitSets[0]`/`[1]`. Per FIDE rules, positions differ if castling
      rights or en-passant availability differ, so this can over-count repetitions (calls a draw slightly
      earlier than a strict arbiter would) in scenarios like: a rook makes a pointless round trip (losing
      castling rights) or an available en-passant capture is declined, and the same piece placement later
      recurs. Discussed and intentionally left as-is: both scenarios require a genuinely senseless move
      (a round trip with no positional point, or declining a capture) to set up, the failure mode only
      ever biases toward an early draw claim (never a blunder or missed repetition), and it's judged not
      worth the churn relative to how rarely it'd actually bite in practice.

- [ ] **Console `MoveValidator` allows castling out of check in some positions.** `BoardLibrary/MoveValidator/MoveValidator.cpp`,
      `validateKing()`'s castling branch checks only the pass-through square and relies on the caller's
      post-move check for the landing square; it never checks whether the king is currently in check on
      its home square. Only affects human move input in `ConsoleApplication`; `MoveGeneratorV2` itself is
      unaffected (it never offers castling while in check).
      Fix: add `not CheckChecker::isCheckOn(*ctx.board, ctx.pieceColor)` to the castling condition.

## Strength-affecting design gaps

- [ ] **King safety evaluation term is disabled — deferred.** `Evaluators/StandardEvaluator/evaluator.cpp:119` —
      `king_safety::evaluate(...)` is commented out. Checked blame: it was added *already commented out* in
      the same commit that introduced `Evaluators/Common/KingSafety.hpp` (`c76d96c`) — never actually tried
      live, not something disabled after a regression. Reviewed the implementation: it's sound (no bug, sign
      convention matches the rest of the evaluator) but narrow — it's a kingside-castling pawn-shield bonus
      only (no queenside coverage, no penalty for an exposed/central king), and its coefficients (20/20/30/20)
      look like untuned placeholder guesses, never run through `CoefficientAdjuster.py`. Decision: hold off
      enabling until it can be validated with `StrenghtComparator.py` (needs `stockfish` + `python-chess`,
      not available in this sandbox) — suspected it may not move the needle much given how narrow it is.

## Minor / lower impact

- [ ] Castling rights aren't revoked when a rook is *captured* on its home square (`MoveApplier.cpp`'s
      `revokeCastlingRights` only looks at the moving piece's source square). Harmless for legality
      (move generation re-checks the rook is physically present) but pollutes the Zobrist hash with a
      stale bit, quietly hurting transposition-table hit rate.
- [ ] Transposition table (`FullCachedEngine` / `non_std::containers::HashMap`) has no aging/replacement
      policy; memory grows unbounded until an explicit `clear()`. Fine for short searches, less fine for a
      long/deep think.
- [ ] `evaluatePositionAvx` (`evaluator.cpp`) is dead code (`USE_AVX` is commented out in the top-level
      `CMakeLists.txt`) and, per the author's own comment, also incomplete (its feature vector skips pawn
      structure/square tables). Either finish it or delete it.
- [ ] Inconsistent 50-move-rule thresholds: search-time draw check uses 150 half-moves
      (`ResultEvaluator.cpp:80`), the leaf-evaluator's check uses 100 (`ResultEvaluator.cpp:120`). May be
      intentional (75-move auto-draw vs. 50-move claim) — worth confirming intent and documenting it.
