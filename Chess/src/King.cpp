#include "King.h"
#include <cmath>

bool King::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    if (rowDiff > 1 || colDiff > 1)
        return false;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board[r][c];
            if (p && p->getColor() != isWhite) {
                if (p->isLegalMove(r, c, destRow, destCol, board)) {
                    return false;
                }
            }
        }
    }

    return true;
}
