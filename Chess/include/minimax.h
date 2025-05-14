#pragma once

#include "Board.h"
#include <vector>
#include <string>
using namespace std;

int evaluateBoard(const Board& board, bool isWhite);
vector<string> generateLegalMoves(Board& board, bool isWhite);
int minimax(Board& board, int depth, bool maximizing, bool isWhite);
vector<pair<int, string>> getBestMoves(string boardStr, bool isWhite, int depth);

