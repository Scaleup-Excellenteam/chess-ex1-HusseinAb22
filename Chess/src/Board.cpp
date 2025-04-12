#include "Board.h"
#include <cctype>  // for std::tolower

Board::Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            boardMove[i][j] = nullptr;
}

Board::~Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete boardMove[i][j];
}

void Board::loadFromString(const std::string& boardString) {
    if (boardString.size() != 64)
        return; // handle invalid input

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            int index = i * 8 + j;
            char symbol = boardString[index];

            delete boardMove[i][j];  // Clear old piece if any
            boardMove[i][j] = nullptr;

            char lower = std::tolower(symbol);
            bool turn = isupper(symbol) ? true : false;

            switch (lower) {
                case 'r':
                    boardMove[i][j] = new Rook(turn);
                break;
                case 'k':
                    boardMove[i][j] = new King(turn);
                break;
                case 'b':
                    boardMove[i][j] = new Bishop(turn);
                break;
                case 'q':
                    boardMove[i][j] = new Queen(turn);
                break;
                case 'n':
                    boardMove[i][j] = new Knight(turn);
                break;
                case 'p':
                    boardMove[i][j] = new Pawn(turn);
                break;
                case '#':
                default:
                    boardMove[i][j] = nullptr;
                break;
            }
        }
}

bool Board::canMove(char color) {
    // Stubbed — logic depends on your piece movement & check detection
    return true;
}

std::string Board::getBoardString() const {
    std::string result;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            result += (boardMove[i][j] ? boardMove[i][j]->getPiece() : '#');
    return result;
}
