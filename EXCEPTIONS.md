# Exception Handling in Chess Project

We define two custom exception types inheriting from `std::runtime_error` to handle invalid moves and board-state errors:

---

## `IllegalMoveException`

* **Purpose**: Signals that the player attempted an illegal chess move.
* **When Thrown**: In `validateMove(...)` when:

    * No piece exists at the source square.
    * The piece belongs to the opponent.
    * The destination square is occupied by the player's own piece.
    * The move pattern is not valid for the piece type.
* **Usage Example**:

  ```cpp
  if (!srcPiece)
      throw IllegalMoveException("no piece at " + move.substr(0,2));
  ```
* **Catch It In**: The main game loop to inform the user without crashing.

---

## `BoardStateException`

* **Purpose**: Indicates an inconsistency or invalid state of the board, beyond simple move legality.
* **When Thrown**: Can be used for future checks such as:

    * Leaving the king in check after a move.
    * Creating impossible board configurations.
* **Usage Example**:

  ```cpp
  if (kingRow < 0)
      throw BoardStateException("king not found on board");
  ```
* **Catch It In**: The main game loop or a higher-level controller to abort or reset.

---

## Integration in `main.cpp`

Wrap your validation step in `try`/`catch` blocks:

```cpp
try {
    int code = validateMove(res, board, isWhite);
    a.setCodeResponse(code);
} catch (const IllegalMoveException& e) {
    std::cout << e.what() << std::endl;
} catch (const BoardStateException& e) {
    std::cout << e.what() << std::endl;
}
```

This ensures the program reports errors gracefully and continues running.

---

### Summary

By centralizing invalid-move detection and board-state validation in exception types, we:

* Keep the core logic clean and focused.
* Provide clear error messages to the player.
* Satisfy the exercise requirement of defining and using two distinct exception classes.
