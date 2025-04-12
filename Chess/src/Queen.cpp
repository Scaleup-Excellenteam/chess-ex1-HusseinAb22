#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

bool Queen::areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    Rook rookHelper(isWhite);
    Bishop bishopHelper(isWhite);

    return rookHelper.areSquaresLegal(srcRow, srcCol, destRow, destCol, boardMove) ||
           bishopHelper.areSquaresLegal(srcRow, srcCol, destRow, destCol, boardMove);
}
