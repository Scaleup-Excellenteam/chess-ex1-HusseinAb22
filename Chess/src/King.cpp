#include "King.h"
#include <cmath>

bool King::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    return rowDiff <= 1 && colDiff <= 1;
}