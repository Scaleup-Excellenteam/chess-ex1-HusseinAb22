#include "Bishop.h"
#include "MoveValidator.h"

bool Bishop::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) {
   return  MoveValidator::isBishopPathClear(srcRow, srcCol, destRow, destCol, boardMove);
}
