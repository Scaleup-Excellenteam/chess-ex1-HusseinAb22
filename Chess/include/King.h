#pragma once
#include "Chess.h"
#include "Piece.h"

class King : public Piece {
    public:
        King(bool isWhite): Piece(isWhite) {}
        ~King() {}
        bool areSquaresLegal(int, int, int, int, Piece *boardMove[8][8]) override;
    private:
        char getPiece() const override { return isWhite ? 'K' : 'k'; }

};
