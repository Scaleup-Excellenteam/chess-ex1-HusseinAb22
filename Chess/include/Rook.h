#pragma once

#include "Piece.h"

class Rook: public Piece {
    public:
        Rook(bool isWhite): Piece(isWhite) {}
        ~Rook() {}
        bool areSquaresLegal(int, int, int, int, Piece *boardMove[8][8]) override;
    private:
        char getPiece() const override { return isWhite ? 'R' : 'r'; }

};
