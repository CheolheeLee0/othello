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
    // ������ ���� ��ġ�� �����Ѵ�. {{0,1}. {0,2}} ����
    std::vector<std::vector<int>> discs_to_flip;

    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // 8���� �����¿�(4��), �밢�� ����(4��)�� ��ȭ��
    int surroundingPosDeltas[8][2] =
    { {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1} };

    // 8���� ���⿡ ���Ͽ� 
    for (auto deltas : surroundingPosDeltas) {
        // ��ȭ���� ���� ���� row, col ��
        int curr_row = row + deltas[0];
        int curr_col = col + deltas[1];

        // board�� ������ ����� ���� �����Ѵ�.
        if (curr_row > n_ - 1 || curr_row < 0 || curr_col > n_ - 1 || curr_col < 0)
            continue;

        // ��ȭ�� ��ġ�� char (b or w)
        char char_in_pos = board_[curr_row][curr_col];

        bool flip_this_direction = false;

        if (char_in_pos == otherPlayer) {
            // char_in_pos ���⿡�� ������ ���� ��ġ�� char_in_pos�� �����Ѵ�.
            while (char_in_pos == otherPlayer) {
                curr_row += deltas[0];
                curr_col += deltas[1];

                // check to see if new position is off board
                if (curr_row > n_ - 1 || curr_row < 0 || curr_col > n_ - 1 || curr_col < 0)
                    break;

                // save the character
                char_in_pos = board_[curr_row][curr_col];
            }

            // ������ ���� ��ġ�� �����̸� �����´�.
            if (char_in_pos == player)
                flip_this_direction = true;

            if (flip_this_direction) {
                curr_row = row + deltas[0];
                curr_col = col + deltas[1];
                char_in_pos = board_[curr_row][curr_col];

                // b - w - w - w - b => b - b - b - b - b
                while (char_in_pos == otherPlayer) {
                    // ������ ���� ��ġ�� discs_to_flip�� �߰��Ѵ�.
                    std::vector<int> disc = { curr_row, curr_col };
                    discs_to_flip.push_back(disc);
                    curr_row += deltas[0];
                    curr_col += deltas[1];

                    char_in_pos = board_[curr_row][curr_col];
                }
            }
        }
    }
    // discs_to_flip ���� ��ġ���� �ش��ϴ� ���� ��� �����´�.
    for (auto pos : discs_to_flip)
        board_[pos[0]][pos[1]] = player;
}

bool Board::isFlippable(int row, int col, char player) {
    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // 8���� �����¿�(4��), �밢�� ����(4��)�� ��ȭ��
    int surroundingPosDeltas[8][2] =
    { {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1} };

    // 8���� ���⿡ ���Ͽ�
    for (auto deltas : surroundingPosDeltas) {
        // board ������ �Ѿ�� �����Ѵ�.
        if (row + deltas[0] > n_ - 1 || row + deltas[0] < 0 || col + deltas[1] > n_ - 1 || col + deltas[1] < 0) {
            continue;
        }
        // �ش� ������ �Ÿ��� 1�� ��ġ�� ��
        char char_in_pos = board_[row + deltas[0]][col + deltas[1]];

        // �ش� ������ �Ÿ��� 1�� ��ġ�� ���� ��� ���� ��,
        if (char_in_pos == otherPlayer) {
            int curr_row = row + deltas[0];
            int curr_col = col + deltas[1];

            // �ش� ������ ������ ���� char_in_pos�� �����Ѵ�.
            while (char_in_pos == otherPlayer) {
                curr_row += deltas[0];
                curr_col += deltas[1];

                if (curr_row > n_ - 1 || curr_row < 0 || curr_col > n_ - 1 || curr_row < 0)
                    break;

                char_in_pos = board_[curr_row][curr_col];
            }

            // ������ ���� player�̸�
            if (char_in_pos == player)
                return true;
        }
    }
    return false;
}

void Board::makeMove(int row, int col, char player) {
    // ���� ����
    board_[row][col] = player;
    // �����´�.
    flip(row, col, player);
}

std::vector<std::vector<int>> Board::calculateLegalMoves(char player) {
    std::vector<std::vector<int>> move_list;
    // ��� board�� ��ġ�� ���ؼ�
    for (int i = 0; i < n_; ++i) {
        for (int j = 0; j < n_; ++j) {
            // ���ڸ��� ���ؼ�
            if (board_[i][j] == '-') {
                // (i, j)�� ������ ������ ���� ������ �� �ִ��� Ȯ���ϰ�
                if (isFlippable(i, j, player)) {
                    // ������ �� ������ move_list�� �߰��Ѵ�.
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

    // board�� ������ ����� error
    if (row > n_ - 1 || row < 0 || col > n_ - 1 || col < 0)
        throw std::range_error{ "isLegalMove()" };

    // ���� �̹� ������ ������
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
        std::cout << "�������� �� �� �ִ� ��ǥ��\n";
        auto v = getBlackLegalMoves();
        for (const auto& vec : v) {
            std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
        }
        std::cout << std::endl << std::endl;
    }
    else {
        std::cout << "����� �� �� �ִ� ��ǥ��\n";
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

    std::cout << "������ ���� : " << black_total << '\n';
    std::cout << "��� ���� : " << white_total << '\n';
    if (black_total == white_total) {
        std::cout << "TIE GAME\n";
        return;
    }

    std::cout << ((black_total > white_total) ? "������" : "���") << " �� �̰���ϴ�!\n";
}
