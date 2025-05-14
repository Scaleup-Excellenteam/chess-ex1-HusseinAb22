#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "MoveValidator.h"
bool Queen::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
    return MoveValidator::isRookPathClear(srcRow, srcCol, destRow, destCol, boardMove) ||
           MoveValidator::isBishopPathClear(srcRow, srcCol, destRow, destCol, boardMove);
}
