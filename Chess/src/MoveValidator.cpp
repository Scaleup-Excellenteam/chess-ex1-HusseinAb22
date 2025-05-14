#include "MoveValidator.h"
#include "Piece.h"
#include <cmath>
#include "Board.h"

bool MoveValidator::isRookPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    if (srcRow == destRow)
    {
        int colOffset = (destCol - srcCol > 0) ? 1 : -1;
        for (int checkCol = srcCol + colOffset;
             checkCol != destCol;
             checkCol += colOffset)
        {
            if (boardMove[srcRow][checkCol] != nullptr)
                return false;
        }
        return true;
    }
    if (destCol == srcCol)
    {
        int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
        for (int checkRow = srcRow + rowOffset; checkRow != destRow; checkRow += rowOffset)
        {
            if (boardMove[checkRow][srcCol] != nullptr)
                return false;
        }
        return true;
    }
    return false;
}

bool MoveValidator::isBishopPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
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
