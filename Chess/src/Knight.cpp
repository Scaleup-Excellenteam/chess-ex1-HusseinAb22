#include "Knight.h"
#include <cmath>

bool Knight::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
