#include "GameValidator.h"
#include "Board.h"
#include "Piece.h"
#include "minimax.h"
#include <iostream>

int validateMove(const std::string& move, std::string& board, bool turn, bool isSimulating) {
    int srcRow = move[0] - 'a';
    int srcCol = move[1] - '1';
    int destRow = move[2] - 'a';
    int destCol = move[3] - '1';

    Board boardObj;
    boardObj.loadFromString(board);

    Piece* srcPiece = boardObj.boardMove[srcRow][srcCol];
    if (srcPiece == nullptr)
        return 11;

    bool isWhite = srcPiece->getColor();
    if (isWhite != turn)
        return 12;

    Piece* destPiece = boardObj.boardMove[destRow][destCol];
    if (destPiece && destPiece->getColor() == srcPiece->getColor())
        return 13;

    if (!srcPiece->isLegalMove(srcRow, srcCol, destRow, destCol, boardObj.boardMove))
        return 21;

    Piece* captured = destPiece;
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

    if (kingRow != -1) {
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
    }

    // Handle pawn promotion if not simulating
    if (Pawn* p = dynamic_cast<Pawn*>(srcPiece)) {
        if (!isSimulating && ((p->getColor() && destRow == 0) || (!p->getColor() && destRow == 7))) {
            char promoteTo;
            std::cout << "Promote pawn to (Q/R/B/N): ";
            std::cin >> promoteTo;
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

void run(std::string& board, bool isWhite, int depth, int numThreads) {
    auto topMoves = getBestMoves(board, isWhite, depth, numThreads);
    std::cout << "\nTop " << topMoves.size() << " Suggestions:\n" << topMoves << std::endl;
}