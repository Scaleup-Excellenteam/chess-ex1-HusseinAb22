#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    ~Bishop() {}
    bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) override;
private:
    char getPiece() const override { return isWhite ? 'B' : 'b'; }

};
