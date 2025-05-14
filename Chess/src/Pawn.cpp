#include "Pawn.h"
#include <cmath>
#include <iostream>
#include <ostream>

#include "Bishop.h"
#include "Exceptions.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"

bool Pawn::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
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
Piece* Pawn::promote(char promoteTo, bool color) {
    switch (promoteTo) {
        case 'Q': case 'q': return new Queen(color);
        case 'R': case 'r': return new Rook(color);
        case 'B': case 'b': return new Bishop(color);
        case 'N': case 'n': return new Knight(color);
        default:
            throw IllegalMoveException("Invalid promotion piece. Use Q, R, B, or N.");
    }
}
