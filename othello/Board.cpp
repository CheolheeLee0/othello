#include "Board.h"

Board::Board(int n) : n_{n} {
	board_ = new char* [n];
	for (int i = 0; i < n; i++)
	{
		board_[i] = new char[n];
		memset(board_[i], '-', sizeof(char) * n);
	}
	board_[n / 2 - 1][n / 2 - 1] = 'w';
	board_[n / 2][n / 2] = 'w';
	board_[n / 2 - 1][n / 2] = 'b';
	board_[n / 2][n / 2 - 1] = 'b';
}

void Board::flip(int row, int col, char player) {
    // 뒤집힐 돌의 위치를 저장한다. {{0,1}. {0,2}} 형태
    std::vector<std::vector<int>> discs_to_flip;

    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // 8개의 상하좌우(4개), 대각선 방향(4개)의 변화값
    int surroundingPosDeltas[8][2] =
    { {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1} };

    // 8개의 방향에 대하여 
    for (auto deltas : surroundingPosDeltas) {
        // 변화값을 더한 후의 row, col 값
        int curr_row = row + deltas[0];
        int curr_col = col + deltas[1];

        // board의 범위를 벗어나는 돌은 무시한다.
        if (curr_row > n_ - 1 || curr_row < 0 || curr_col > n_ - 1 || curr_col < 0)
            continue;

        // 변화된 위치의 char (b or w)
        char char_in_pos = board_[curr_row][curr_col];

        bool flip_this_direction = false;

        if (char_in_pos == otherPlayer) {
            // char_in_pos 방향에서 마지막 돌의 위치를 char_in_pos에 저장한다.
            while (char_in_pos == otherPlayer) {
                curr_row += deltas[0];
                curr_col += deltas[1];

                // check to see if new position is off board
                if (curr_row > n_ - 1 || curr_row < 0 || curr_col > n_ - 1 || curr_col < 0)
                    break;

                // save the character
                char_in_pos = board_[curr_row][curr_col];
            }

            // 마지막 돌의 위치가 본인이면 뒤집는다.
            if (char_in_pos == player)
                flip_this_direction = true;

            if (flip_this_direction) {
                curr_row = row + deltas[0];
                curr_col = col + deltas[1];
                char_in_pos = board_[curr_row][curr_col];

                // b - w - w - w - b => b - b - b - b - b
                while (char_in_pos == otherPlayer) {
                    // 뒤집을 돌의 위치를 discs_to_flip에 추가한다.
                    std::vector<int> disc = { curr_row, curr_col };
                    discs_to_flip.push_back(disc);
                    curr_row += deltas[0];
                    curr_col += deltas[1];

                    char_in_pos = board_[curr_row][curr_col];
                }
            }
        }
    }
    // discs_to_flip 안의 위치값에 해당하는 값을 모두 뒤집는다.
    for (auto pos : discs_to_flip)
        board_[pos[0]][pos[1]] = player;
}

bool Board::isFlippable(int row, int col, char player) {
    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // 8개의 상하좌우(4개), 대각선 방향(4개)의 변화값
    int surroundingPosDeltas[8][2] =
    { {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1} };

    // 8개의 방향에 대하여
    for (auto deltas : surroundingPosDeltas) {
        // board 범위를 넘어가면 무시한다.
        if (row + deltas[0] > n_ - 1 || row + deltas[0] < 0 || col + deltas[1] > n_ - 1 || col + deltas[1] < 0) {
            continue;
        }
        // 해당 방향의 거리가 1인 위치의 돌
        char char_in_pos = board_[row + deltas[0]][col + deltas[1]];

        // 해당 방향의 거리가 1인 위치의 돌이 상대 돌일 때,
        if (char_in_pos == otherPlayer) {
            int curr_row = row + deltas[0];
            int curr_col = col + deltas[1];

            // 해당 방향의 마지막 돌을 char_in_pos에 저장한다.
            while (char_in_pos == otherPlayer) {
                curr_row += deltas[0];
                curr_col += deltas[1];

                if (curr_row > n_ - 1 || curr_row < 0 || curr_col > n_ - 1 || curr_row < 0)
                    break;

                char_in_pos = board_[curr_row][curr_col];
            }

            // 마지막 돌이 player이면
            if (char_in_pos == player)
                return true;
        }
    }
    return false;
}

void Board::makeMove(int row, int col, char player) {
    // 돌을 놓고
    board_[row][col] = player;
    // 뒤집는다.
    flip(row, col, player);
}

std::vector<std::vector<int>> Board::calculateLegalMoves(char player) {
    std::vector<std::vector<int>> move_list;
    // 모든 board의 위치에 대해서
    for (int i = 0; i < n_; ++i) {
        for (int j = 0; j < n_; ++j) {
            // 빈자리에 대해서
            if (board_[i][j] == '-') {
                // (i, j)에 놓으면 상대방의 말을 뒤집을 수 있는지 확인하고
                if (isFlippable(i, j, player)) {
                    // 뒤집을 수 있으면 move_list에 추가한다.
                    std::vector<int> move = { i, j };
                    move_list.push_back(move);
                }
            }
        }
    }
    return move_list;
}

bool Board::isLegalMove(std::vector<std::vector<int>> move_list, int row, int col, char player) {
    std::vector<int> proposedMove = { row, col };

    // board의 범위를 벗어나면 error
    if (row > n_ - 1 || row < 0 || col > n_ - 1 || col < 0)
        throw std::range_error{ "isLegalMove()" };

    // 돌이 이미 놓여져 있으면
    if (board_[row][col] != '-') {
        return false;
    }
    if (std::find(move_list.begin(), move_list.end(), proposedMove) != move_list.end()) {
        return true;
    }
    return false;
}

std::vector<std::vector<int>> Board::getBlackLegalMoves() {
    return calculateLegalMoves('b');
}

std::vector<std::vector<int>> Board::getWhiteLegalMoves() {
    return calculateLegalMoves('w');
}

void Board::printLegalMoves(char player) {
    if (player == 'b') {
        std::cout << "검은색이 갈 수 있는 좌표는\n";
        auto v = getBlackLegalMoves();
        for (const auto& vec : v) {
            std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
        }
        std::cout << std::endl << std::endl;
    }
    else {
        std::cout << "흰색이 갈 수 있는 좌표는\n";
        auto x = getWhiteLegalMoves();
        for (const auto& vec : x) {
            std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
        }
        std::cout << std::endl;
    }
}

void Board::printLegalMoves(std::vector<std::vector<int>> move_list) {
    for (const auto& vec : move_list) {
        std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
    }
    std::cout << std::endl;
}

void Board::printAllBoard() {
    std::cout << "   0";
    for (int i = 1; i < n_; ++i) {
        std::cout << "  " << (i);
    }
    std::cout << std::endl;
    for (int i = 0; i < n_; ++i) {
        std::cout << (i) << "  ";
        for (int j = 0; j < n_; ++j) {
            std::cout << board_[i][j] << "  ";
        }
        std::cout << '\n';
    }
}

bool Board::isGameOver() {
    return getBlackLegalMoves().empty() && getWhiteLegalMoves().empty();
}

int Board::getScore(char player) {
    int total = 0;
    for (int i = 0; i < n_; ++i)
        for (int j = 0; j < n_; ++j)
            if (board_[i][j] == player)
                total += 1;

    return total;
}

void Board::printWinner() {
    int white_total = getScore('w');
    int black_total = getScore('b');

    std::cout << "검은색 개수 : " << black_total << '\n';
    std::cout << "흰색 개수 : " << white_total << '\n';
    if (black_total == white_total) {
        std::cout << "TIE GAME\n";
        return;
    }

    std::cout << ((black_total > white_total) ? "검은색" : "흰색") << " 이 이겼습니다!\n";
}
