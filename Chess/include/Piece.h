#pragma once


using namespace std;

class Piece {
public:
    Piece(bool isWhite): isWhite(isWhite) {}
    bool getColor() const { return isWhite; }
    virtual bool areSquaresLegal(int, int, int, int, Piece *boardMove[8][8]) = 0;
    virtual char getPiece() const = 0;
    virtual ~Piece() {}

protected:

    bool isWhite;
};



