#pragma once
#include <string>

int validateMove(const std::string& move, std::string& board, bool turn, bool isSimulating);
void run(std::string& board, bool isWhite, int depth, int numThreads);

