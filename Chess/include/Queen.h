#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool isWhite): Piece(isWhite) {}
    ~Queen() {}
    bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) override;
private:
    char getPiece() const override { return isWhite ? 'Q' : 'q'; }

};
