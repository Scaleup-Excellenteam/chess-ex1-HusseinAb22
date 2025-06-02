#include "Chess.h"
#include "GameValidator.h"
#include "Exceptions.h"
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include "minimax.h"
using namespace std;

// Forward declaration
void run(string& board, bool isWhite, int depth, int numThreads);

void runAutomaticGame(int depth, int numThreads) {
    cout << "\n--- Running Automatic Game ---" << endl;
    cout << "Depth: " << depth << ", Threads: " << numThreads << endl;

    string board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR";
    bool isWhite = true;
    const int totalMoves = 8; // 4 moves per side

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < totalMoves; ++i) {
        auto best_moves = getBestMoves(board, isWhite, depth, numThreads);
        if (best_moves.empty()) {
            cout << "No legal moves. Game over." << endl;
            break;
        }
        string best_move = best_moves.front().second;
        cout << "Move " << i + 1 << ": " << best_move << endl;

        int codeResponse = validateMove(best_move, board, isWhite, false);
        if (codeResponse >= 41) {
            isWhite = !isWhite;
        } else {
            cout << "Error: Invalid move generated: " << best_move << " with code " << codeResponse << endl;
            break;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout << "Automatic game finished in " << diff.count() << " seconds." << endl;
}

void runManualGame(int depth, int numThreads) {
    string board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR";
    Chess a(board);
    int codeResponse = 0;
    string res;
    bool isWhite = true;

    while (true) {

        run(board, isWhite, depth, numThreads);

        res = a.getInput();
        if (res == "exit") break;

        try {
            codeResponse = validateMove(res, board, isWhite, false);

            if (codeResponse >= 41) {
                isWhite = !isWhite;
            }

            a.setCodeResponse(codeResponse);
        } catch (const std::exception& e) {
            cout << "An error occurred: " << e.what() << endl;
        }
    }
}

int main() {
    int depth = 3;
    string mode;

    cout << "Enter search depth for move suggestions (e.g., 3): ";
    cin >> depth;

    cout << "Enter 'auto' for benchmark or 'manual' for interactive play: ";
    cin >> mode;

    if (mode == "auto") {
        runAutomaticGame(depth, 1);
        runAutomaticGame(depth, 2);
        runAutomaticGame(depth, 4);
        runAutomaticGame(depth, 8);
    } else {
        int numThreads = 4;
        cout << "Enter number of threads for suggestions (e.g., 4): ";
        cin >> numThreads;
        runManualGame(depth, numThreads);
    }

    cout << endl << "Exiting." << endl;
    return 0;
}