#pragma once
#include "Piece.h"


class Knight : public Piece {
public:
    Knight(bool isWhite): Piece(isWhite) {}
    ~Knight() {}
    bool isLegalMove(int, int, int, int, Piece *boardMove[8][8]) override;
    char getPiece() const override { return isWhite ? 'N' : 'n'; }


};

