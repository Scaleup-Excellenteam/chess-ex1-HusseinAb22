#include "Bishop.h"
#include <cmath> // for abs

bool Bishop::areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    if (std::abs(destRow - srcRow) != std::abs(destCol - srcCol)) {
        return false;
    }

    int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
    int colOffset = (destCol - srcCol > 0) ? 1 : -1;

    int checkRow = srcRow + rowOffset;
    int checkCol = srcCol + colOffset;

    while (checkRow != destRow && checkCol != destCol) {
        if (boardMove[checkRow][checkCol] != nullptr)
            return false;

        checkRow += rowOffset;
        checkCol += colOffset;
    }

    return true;
}
