#include "Pawn.h"
#include <cmath>
#include <iostream>
#include <ostream>

bool Pawn::areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    int direction = isWhite ? 1 : -1;
    int startRow = isWhite ? 1 : 6;


    if (srcCol == destCol) {
        if (destRow == srcRow + direction && boardMove[destRow][destCol] == nullptr)
            return true;

        // First move - 2 steps forward
        if (srcRow == startRow && destRow == srcRow + 2 * direction &&
            boardMove[srcRow + direction][srcCol] == nullptr &&
            boardMove[destRow][destCol] == nullptr)
            return true;


    }

    // Capture diagonally
    if (std::abs(destCol - srcCol) == 1 && destRow == srcRow + direction &&
        boardMove[destRow][destCol] != nullptr &&
        boardMove[destRow][destCol]->getColor() != isWhite)
        return true;

    return false;
}
