#include "Rook.h"
#include "MoveValidator.h"

bool Rook::isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8])
{
    return MoveValidator::isRookPathClear(srcRow, srcCol, destRow, destCol, boardMove);
}

