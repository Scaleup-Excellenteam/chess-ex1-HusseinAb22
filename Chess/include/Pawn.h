#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    ~Pawn() {}
    bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* boardMove[8][8]) override;

    static Piece *promote(char promoteTo, bool color);

    char getPiece() const override { return isWhite ? 'P' : 'p'; }
};
