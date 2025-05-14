# Chess Move Suggestion Algorithm: Minimax Summary

## Overview

This project implements a naive depth-limited minimax algorithm to suggest the best moves in a text-based chess game. The goal is to explore all possible legal moves for a player and evaluate each based on material gain/loss and positional threats. Up to three turns (plies) deep are considered.

---

## Algorithm Steps

### 1. Generating All Legal Moves

* For each piece on the current player's side:

    * All 64 destination squares are evaluated.
    * If the move is valid (`validateMove(...)` returns a legal code), it's added to the list of options.

### 2. Scoring a Move

* At **depth 0**, a move is scored based on:

    * Capturing an opponent's piece (adds points).
    * Threatening a stronger piece (adds points).
    * Leaving our piece in danger from a weaker piece (subtracts points).
* At **depth 1**, simulate all opponent replies. Subtract the opponent's best reply score.
* At **depth 2**, simulate our best response to the opponent's reply and add that score.

### 3. Prioritizing Moves

* All moves are scored and stored in a custom `PriorityQueue<pair<int,string>>`, sorted by score.
* Only the **top 5** moves are retained.
* Before the player's turn, the **top 3** moves are printed using an overloaded `<<` operator.

---

## Time Complexity

Let `M` be the average number of legal moves (≈20–30 per position).

| Depth | Description                        | Complexity |
| ----- | ---------------------------------- | ---------- |
| 0     | Static evaluation only             | O(M)       |
| 1     | Best reply from opponent           | O(M^2)     |
| 2     | Our best reply after opponent move | O(M^3)     |

Overall time complexity at **depth 2** is **O(M^3)** due to evaluating every root move + all opponent replies + all our next replies.

---

## Example Output

```
Top 3 Suggestions:
e2e4 (15)
d2d4 (12)
f2f4 (10)
```

Each suggestion includes the move and its corresponding score based on simulated outcomes.

---

## Notes

* The `PriorityQueue` ensures only the top scoring 5 moves are retained.
* The move suggestion engine does not use alpha-beta pruning—every possible line up to the selected depth is evaluated.
* This is a pure naive minimax: no caching, pruning, or heuristics beyond static board evaluation.

---

## Conclusion

This implementation satisfies the assignment by:

* Fully generating and validating all legal moves.
* Evaluating each with a consistent scoring function.
* Applying a 3-ply minimax structure.
* Displaying the best 3 moves via a sorted queue and `<<` overload.

The approach is simple but exhaustive, and serves as a solid baseline for understanding full chess engine search techniques.
