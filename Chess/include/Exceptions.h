#pragma once

#include <stdexcept>
#include <string>

// Thrown when an attempted chess move is invalid (e.g., no piece at source, illegal pattern)
class IllegalMoveException : public std::runtime_error {
public:
    explicit IllegalMoveException(const std::string& msg)
        : std::runtime_error(std::string("Illegal move: ") + msg) {}
};

// Thrown when the board reaches an invalid or inconsistent state
class BoardStateException : public std::runtime_error {
public:
    explicit BoardStateException(const std::string& msg)
        : std::runtime_error(std::string("Board state error: ") + msg) {}
};
