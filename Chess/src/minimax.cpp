#include "minimax.h"
#include "Board.h"
#include "PriorityQueue.h"
#include "GameValidator.h"
#include "ThreadPool.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <limits>
#include <future>

// ... (evaluateBoard and generateLegalMoves remain the same) ...
static map<char, int> pieceValue = {
    {'p', 1}, {'n', 3}, {'b', 3}, {'r', 5}, {'q', 9}, {'k', 1000},
    {'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 1000}
};
int evaluateBoard(Board& board, bool isWhite) {
    int score = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.boardMove[r][c];
            if (!p) continue;
            char ch = p->getPiece();
            int base = pieceValue[ch];
            bool color = p->getColor();

            score += (color == isWhite ? 1 : -1) * base;

            if ((r == 3 || r == 4) && (c == 3 || c == 4)) {
                score += (color == isWhite ? 1 : -1) * 2;
            }

            for (int r2 = 0; r2 < 8; ++r2) {
                for (int c2 = 0; c2 < 8; ++c2) {
                    Piece* target = board.boardMove[r2][c2];
                    if (target && target->getColor() != color) {
                        if (p->isLegalMove(r, c, r2, c2, board.boardMove)) {
                            int targetValue = pieceValue[target->getPiece()];
                            if (color == isWhite) {
                                score += targetValue + 3;
                                if (p->getPiece() == 'R' || p->getPiece() == 'Q') {
                                    if (r == r2 || c == c2) score += 2;
                                }
                            } else {
                                score -= targetValue / 2;
                            }
                        }
                    }
                }
            }

            for (int r3 = 0; r3 < 8; ++r3) {
                for (int c3 = 0; c3 < 8; ++c3) {
                    Piece* enemy = board.boardMove[r3][c3];
                    if (enemy && enemy->getColor() != color) {
                        if (enemy->isLegalMove(r3, c3, r, c, board.boardMove)) {
                            int attackerValue = pieceValue[enemy->getPiece()];
                            if (attackerValue < base) {
                                if (color == isWhite)
                                    score -= attackerValue;
                                else
                                    score += attackerValue;
                            }
                        }
                    }
                }
            }
        }
    }

    int myCoverage = 0, oppCoverage = 0;
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int c1 = 0; c1 < 8; ++c1) {
            Piece* p = board.boardMove[r1][c1];
            if (!p) continue;
            for (int r2 = 0; r2 < 8; ++r2) {
                for (int c2 = 0; c2 < 8; ++c2) {
                    if (p->isLegalMove(r1, c1, r2, c2, board.boardMove)) {
                        if (p->getColor() == isWhite)
                            myCoverage++;
                        else
                            oppCoverage++;
                    }
                }
            }
        }
    }
    score += myCoverage - oppCoverage;

    return score;
}


vector<string> generateLegalMoves(Board& board, bool isWhite) {
    vector<string> moves;
    for (int srcRow = 0; srcRow < 8; ++srcRow) {
        for (int srcCol = 0; srcCol < 8; ++srcCol) {
            Piece* piece = board.boardMove[srcRow][srcCol];
            if (piece && piece->getColor() == isWhite) {
                for (int destRow = 0; destRow < 8; ++destRow) {
                    for (int destCol = 0; destCol < 8; ++destCol) {
                        string move = string(1, 'a' + srcRow) + string(1, '1' + srcCol) +
                                       string(1, 'a' + destRow) + string(1, '1' + destCol);

                        string tempBoard = board.getBoardString();
                        if (move.size() == 4 && tempBoard.size() == 64) {
                            try {
                                int result = validateMove(move, tempBoard, isWhite,true);
                                if (result >= 41) {
                                    moves.push_back(move);

                                }

                            } catch (...) {
                                cerr << "Exception caught during move validation: " << move << endl;
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int minimax(Board& board, int depth, bool maximizing, bool isWhite) {
    if (depth == 0)
        return evaluateBoard(board, isWhite);

    vector<string> moves = generateLegalMoves(board, maximizing ? isWhite : !isWhite);
    int bestScore = maximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();

    for (const string& move : moves) {
        string boardStr = board.getBoardString();
        Board tempBoard;
        tempBoard.loadFromString(boardStr);
        int result = validateMove(move, boardStr, maximizing ? isWhite : !isWhite,true);
        if (result < 41) continue;
        tempBoard.loadFromString(boardStr);

        int score = minimax(tempBoard, depth - 1, !maximizing, isWhite);
        bestScore = maximizing ? max(bestScore, score) : min(bestScore, score);
    }
    return bestScore;
}


vector<pair<int, string>> getBestMoves(string boardStr, bool isWhite, int depth, int numThreads) {
    Board board;
    board.loadFromString(boardStr);
    auto moves = generateLegalMoves(board, isWhite);

    PriorityQueue<pair<int, string>> pq;

    if (numThreads > 1 && moves.size() > 1) {
        ThreadPool pool(numThreads);
        std::vector<std::future<void>> results;

        for (const auto& mv : moves) {
            results.emplace_back(
                pool.enqueue([&pq, boardStr, isWhite, depth, mv] {
                    string tmp = boardStr;
                    int res = validateMove(mv, tmp, isWhite, true);
                    if (res >= 41) {
                        Board sim;
                        sim.loadFromString(tmp);
                        int score = minimax(sim, depth - 1, false, isWhite);
                        pq.push({score, mv});
                    }
                })
            );
        }

        for (auto&& result : results) {
            result.get(); // Wait for all tasks to complete
        }
    } else { // Single-threaded execution
        for (const auto& mv : moves) {
            string tmp = boardStr;
            int res = validateMove(mv, tmp, isWhite, true);
            if (res >= 41) {
                Board sim;
                sim.loadFromString(tmp);
                int score = minimax(sim, depth - 1, false, isWhite);
                pq.push({score, mv});
            }
        }
    }

    return pq.to_vector();
}