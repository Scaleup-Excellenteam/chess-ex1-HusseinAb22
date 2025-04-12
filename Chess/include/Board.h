#pragma once

#include "Piece.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"

#include <string>

class Board {
public:
    Board();
    ~Board();

    void loadFromString(const std::string& boardString);
    bool canMove(char color);

    Piece* boardMove[8][8];
    string getBoardString() const;

    // Prevent copying (because of raw pointers)
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
};
