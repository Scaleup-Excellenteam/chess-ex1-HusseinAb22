#pragma once


using namespace std;

class Piece {
public:
    Piece(bool isWhite): isWhite(isWhite) {}

    Piece(bool isWhite, int row, int col)
       : isWhite(isWhite), row(row), col(col) {}

    bool getColor() const { return isWhite; }

    int getRow() const { return row; }
    int getCol() const { return col; }

    void setPosition(int r, int c) {
        row = r;
        col = c;
    }

    virtual bool isLegalMove(int, int, int, int, Piece *boardMove[8][8]) = 0;
    virtual char getPiece() const = 0;
    virtual ~Piece() {}

protected:

    bool isWhite;
    int row, col;
};



