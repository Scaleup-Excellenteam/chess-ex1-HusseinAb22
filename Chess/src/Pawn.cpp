// In Pawn.cpp
#include "Pawn.h"
#include <cmath>
#include <iostream>
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Exceptions.h"

bool Pawn::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    int direction;
    int startRow;

    if (isWhite) { // White pawns
        direction = -1;
        startRow = 6;
    } else { // Black pawns
        direction = 1;
        startRow = 1;
    }

    if (srcCol == destCol) {
        if (destRow == srcRow + direction && boardMove[destRow][destCol] == nullptr) {
            return true;
        }
        if (srcRow == startRow && destRow == srcRow + 2 * direction &&
            boardMove[srcRow + direction][destCol] == nullptr &&
            boardMove[destRow][destCol] == nullptr) {
            return true;
        }
    }

    if (std::abs(destCol - srcCol) == 1 && destRow == srcRow + direction) {
        if (boardMove[destRow][destCol] != nullptr &&
            boardMove[destRow][destCol]->getColor() != this->isWhite) {
            return true;
        }
    }

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