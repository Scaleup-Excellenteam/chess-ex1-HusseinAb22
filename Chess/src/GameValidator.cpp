#include "GameValidator.h"
#include "Board.h"
#include "Piece.h"

int validateMove(const string& move, string& board,bool turn) {
    int srcRow = move[0] - 'a';
    int srcCol = move[1] - '1';
    int destRow = move[2] - 'a';
    int destCol = move[3] - '1';

    Board boardObj;
    boardObj.loadFromString(board);



    Piece* srcPiece = boardObj.boardMove[srcRow][srcCol];
    Piece* destPiece = boardObj.boardMove[destRow][destCol];
     if (srcPiece == nullptr)
        return 11;

    bool isWhite = srcPiece->getColor();

    if (isWhite != turn)
        return 12;
    if (destPiece && destPiece->getColor() == srcPiece->getColor())
        return 13;
    if (!srcPiece->areSquaresLegal(srcRow, srcCol, destRow, destCol, boardObj.boardMove))
        return 21;




    boardObj.boardMove[destRow][destCol] = srcPiece;
    boardObj.boardMove[srcRow][srcCol] = nullptr;
    board = boardObj.getBoardString();




    return 42;
}
