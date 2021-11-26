// 201802141 ¿Ã√∂»Ò
#include "common.h"
#pragma once

class Board {
 public:
    Board(int n);
    void flip(int row, int col, char player);
    bool isFlippable(int row, int col, char player);
    void makeMove(int row, int col, char player);
    std::vector<std::vector<int>> calculateLegalMoves(char player);
    bool isLegalMove(std::vector<std::vector<int>> move_list, int row, int col, char player);
    std::vector<std::vector<int>> getBlackLegalMoves();
    std::vector<std::vector<int>> getWhiteLegalMoves();
    void printLegalMoves(char player);
    void printLegalMoves(std::vector<std::vector<int>> move_list);
    void printAllBoard();
    bool isGameOver();
    int getScore(char player);
    void printWinner();
 private:
    const int n_;  // ≈©±‚
    char** board_;  // board
};
