#include "GameValidator.h"
#include "Board.h"
#include "Piece.h"
#include "minimax.h"
#include <iostream>

int validateMove(const string& move, string& board, bool turn, bool isSimulating) {
    int srcRow = move[0] - 'a';
    int srcCol = move[1] - '1';
    int destRow = move[2] - 'a';
    int destCol = move[3] - '1';

    Board boardObj;
    boardObj.loadFromString(board);

    Piece* srcPiece = boardObj.boardMove[srcRow][srcCol];
    Piece* destPiece = boardObj.boardMove[destRow][destCol];

    if (srcPiece == nullptr)
        return 11;
    bool isWhite = srcPiece->getColor();
    if (isWhite != turn)
        return 12;
    if (destPiece && destPiece->getColor() == srcPiece->getColor())
        return 13;
    if (!srcPiece->isLegalMove(srcRow, srcCol, destRow, destCol, boardObj.boardMove))
        return 21;

    Piece* captured = boardObj.boardMove[destRow][destCol];
    boardObj.boardMove[destRow][destCol] = srcPiece;
    boardObj.boardMove[srcRow][srcCol] = nullptr;

    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8 && kingRow == -1; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = boardObj.boardMove[r][c];
            if (p && p->getColor() == isWhite && (p->getPiece() == 'K' || p->getPiece() == 'k')) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* attacker = boardObj.boardMove[r][c];
            if (attacker && attacker->getColor() != isWhite) {
                if (attacker->isLegalMove(r, c, kingRow, kingCol, boardObj.boardMove)) {
                    return 31;
                }
            }
        }
    }

    // Handle pawn promotion
    Piece* moved = boardObj.boardMove[destRow][destCol];

    if (Pawn* p = dynamic_cast<Pawn*>(moved)) {

        if (!isSimulating && ((p->getColor() && destRow == 7) || (!p->getColor() && destRow == 0))) {
            char promoteTo;
            std::cout << "Promote pawn to (Q/R/B/N): ";
            std::cin >> promoteTo;
            boardObj.boardMove[destRow][destCol] = p->promote(promoteTo, p->getColor());
            delete p;
        }
    }


    board = boardObj.getBoardString();

    return 42;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::pair<int,std::string>>& v) {
    for (const auto& [sc, mv] : v)
        os << mv << " (" << sc << ")\n";
    return os;
}

// Updated run function
void run(string& board, bool& isWhite, int depth, int numThreads) {
    auto topMoves = getBestMoves(board, isWhite, depth, numThreads);

    cout << "\nTop " << topMoves.size() << " Suggestions:\n" << topMoves << endl;
}