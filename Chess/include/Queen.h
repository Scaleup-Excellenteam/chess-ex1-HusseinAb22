#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool isWhite): Piece(isWhite) {}
    ~Queen() {}
    bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) override;

    char getPiece() const override { return isWhite ? 'Q' : 'q'; }

};
