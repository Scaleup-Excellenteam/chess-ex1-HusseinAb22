#include "King.h"


bool King::areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    int rowDiff = abs(destRow - srcRow);
    int colDiff = abs(destCol - srcCol);
    return (rowDiff <= 1 && colDiff <= 1);
}
