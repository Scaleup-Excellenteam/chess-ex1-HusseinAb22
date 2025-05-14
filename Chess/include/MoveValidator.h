#pragma once

class Piece;

class MoveValidator {
public:
    static bool isRookPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]);
    static bool isBishopPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]);
};
