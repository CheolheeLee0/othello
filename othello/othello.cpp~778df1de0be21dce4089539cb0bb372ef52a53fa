#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <regex>

const bool PLAY_AI = false; // 상대를 AI로 할 것 인지 또는 player2로 할 것인지 정함
const int MINIMAX_DEPTH = 5; // depth of the game tree search
const bool DEBUG_MODE = false;

// player가 (row, col)위치에 돌을 놓으면 오델로 규칙에 맞는 돌을 뒤집는다.
void flip(char(&board)[8][8], int row, int col, char player) {
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
        if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
            continue;

        // 변화된 위치의 char (b or w)
        char char_in_pos = board[curr_row][curr_col];

        bool flip_this_direction = false;

        if (char_in_pos == otherPlayer) {
            // char_in_pos 방향에서 마지막 돌의 위치를 char_in_pos에 저장한다.
            while (char_in_pos == otherPlayer) {
                curr_row += deltas[0];
                curr_col += deltas[1];

                // check to see if new position is off board
                if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
                    break;

                // save the character
                char_in_pos = board[curr_row][curr_col];
            }

            // 마지막 돌의 위치가 본인이면 뒤집는다.
            if (char_in_pos == player)
                flip_this_direction = true;

            if (flip_this_direction) {
                curr_row = row + deltas[0];
                curr_col = col + deltas[1];
                char_in_pos = board[curr_row][curr_col];

                // b - w - w - w - b => b - b - b - b - b
                while (char_in_pos == otherPlayer) {
                    // 뒤집을 돌의 위치를 discs_to_flip에 추가한다.
                    std::vector<int> disc = { curr_row, curr_col };
                    discs_to_flip.push_back(disc);
                    curr_row += deltas[0];
                    curr_col += deltas[1];

                    char_in_pos = board[curr_row][curr_col];
                }
            }
        }
    }
    // discs_to_flip 안의 위치값에 해당하는 값을 모두 뒤집는다.
    for (auto pos : discs_to_flip)
        board[pos[0]][pos[1]] = player;
}

// (row, col)를 기준으로 거리 상관없이 8개의 방향에 대해서 돌을 놓으면 상대 말을 뒤집을 수 있는지 반환한다. (true, false)
bool isFlippable(char board[8][8], int row, int col, char player) {
    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // 8개의 상하좌우(4개), 대각선 방향(4개)의 변화값
    int surroundingPosDeltas[8][2] =
        { {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1} };

    // 8개의 방향에 대하여
    for (auto deltas : surroundingPosDeltas) {
        // board 범위를 넘어가면 무시한다.
        if (row + deltas[0] > 7 || row + deltas[0] < 0 || col + deltas[1] > 7 || col + deltas[1] < 0) {
            continue;
        }
        // 해당 방향의 거리가 1인 위치의 돌
        char char_in_pos = board[row + deltas[0]][col + deltas[1]];

        // 해당 방향의 거리가 1인 위치의 돌이 상대 돌일 때,
        if (char_in_pos == otherPlayer) {
            int curr_row = row + deltas[0];
            int curr_col = col + deltas[1];
            
            // 해당 방향의 마지막 돌을 char_in_pos에 저장한다.
            while (char_in_pos == otherPlayer) {
                curr_row += deltas[0];
                curr_col += deltas[1];

                if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_row < 0)
                    break;

                char_in_pos = board[curr_row][curr_col];
            }

            // 마지막 돌이 player이면
            if (char_in_pos == player)
                return true;
        }
    }
    return false;
}

// 돌을 (row, col)에 놓는다.
void makeMove(char(&board)[8][8], int row, int col, char player) {
    // 돌을 놓고
    board[row][col] = player;
    // 뒤집는다.
    flip(board, row, col, player);
}

// player의 돌을 놓을 수 있는 자리를 구해 vector로 반환한다.
std::vector<std::vector<int>> calculateLegalMoves(char board[8][8], char player) {
    std::vector<std::vector<int>> move_list;
    // 모든 board의 위치에 대해서
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 빈자리에 대해서
            if (board[i][j] == '-') {
                // (i, j)에 놓으면 상대방의 말을 뒤집을 수 있는지 확인하고
                if (isFlippable(board, i, j, player)) {
                    // 뒤집을 수 있으면 move_list에 추가한다.
                    std::vector<int> move = { i, j };
                    move_list.push_back(move);
                }
            }
        }
    }
    return move_list;
}

// 돌을 놓을 수 있는 자리에 (row, col)가 포함되어 있는지 확인한다.
bool isLegalMove(char board[8][8], std::vector<std::vector<int>> move_list, int row, int col, char player) {
    std::vector<int> proposedMove = { row, col };
   
    // board의 범위를 벗어나면 error
    if (row > 7 || row < 0 || col > 7 || col < 0)
        throw std::range_error{ "isLegalMove()" };

    // 돌이 이미 놓여져 있으면
    if (board[row][col] != '-') {
        return false;
    }
    if (std::find(move_list.begin(), move_list.end(), proposedMove) != move_list.end()) {
        return true;
    }
    return false;
}

// 검은 돌을 놓을 수 있는 자리를 반환
std::vector<std::vector<int>> getBlackLegalMoves(char board[8][8]) {
    return calculateLegalMoves(board, 'b');
}

// 흰 돌을 놓을 수 있는 자리를 반환
std::vector<std::vector<int>> getWhiteLegalMoves(char board[8][8]) {
    return calculateLegalMoves(board, 'w');
}

// player가 돌을 놓을 수 있는 자리를 출력한다.
void printLegalMoves(char board[8][8], char player) {
    if (player == 'b') {
        std::cout << "검은색이 갈 수 있는 좌표는\n";
        auto v = getBlackLegalMoves(board);
        for (const auto& vec : v) {
            std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
        }
        std::cout << std::endl << std::endl;
    }
    else {
        std::cout << "흰색이 갈 수 있는 좌표는\n";
        auto x = getWhiteLegalMoves(board);
        for (const auto& vec : x) {
            std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
        }
        std::cout << std::endl;
    }
}

// move_list에 저장된 돌을 놓을 수 있는 자리를 출력한다.
void printLegalMoves(std::vector<std::vector<int>> move_list) {
    for (const auto& vec : move_list) {
        std::cout << "(" << vec[0] << "," << vec[1] << ")  ";
    }
    std::cout << std::endl;
}

// board를 출력한다.
std::ostream& operator<<(std::ostream& os, const char board[8][8]) {
    std::cout << "   0  1  2  3  4  5  6  7\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << (i) << "  ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << "  ";
        }
        std::cout << '\n';
    }
    return os;
}

// 두 플레이어 모두 돌을 놓을 수 있는 자리가 없으면 게임을 종료한다.
bool isGameOver(char board[8][8]) {
    return getBlackLegalMoves(board).empty() && getWhiteLegalMoves(board).empty();
}

// player의 점수를 계산한다.
int getScore(char board[8][8], char player) {
    int total = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (board[i][j] == player)
                total += 1;

    return total;
}

// 게임의 결과를 출력한다.
void printWinner(char(&board)[8][8]) {
    int white_total = getScore(board, 'w');
    int black_total = getScore(board, 'b');

    std::cout << "검은색 개수 : " << black_total << '\n';
    std::cout << "흰색 개수 : " << white_total << '\n';
    if (black_total == white_total) {
        std::cout << "TIE GAME\n";
        return;
    }

    std::cout << ((black_total > white_total) ? "검은색" : "흰색") << " 이 이겼습니다!\n";
}

// heursitic used to give value to varying states of the game
int heuristic(char board[8][8]) {
    // intialize black and white total
    int b_total = 0;
    int w_total = 0;

    // factor in the amount of moves each player has
    b_total += getBlackLegalMoves(board).size();
    w_total += getWhiteLegalMoves(board).size();

    // factor in the amount of pieces each player has on the board
    b_total += getScore(board, 'b');
    w_total += getScore(board, 'w');

    // factor in the importance of all 4 corners
    if (board[0][0] == 'w') {
        w_total += 10;
    }
    if (board[0][0] == 'b') {
        b_total += 10;
    }
    if (board[7][0] == 'w') {
        w_total += 10;
    }
    if (board[7][0] == 'b') {
        b_total += 10;
    }
    if (board[0][7] == 'w') {
        w_total += 10;
    }
    if (board[0][7] == 'b') {
        b_total += 10;
    }
    if (board[7][7] == 'w') {
        w_total += 10;
    }
    if (board[7][7] == 'b') {
        b_total += 10;
    }
    // subtract white's total from black, let black be the maximizer
    return (b_total - w_total);
}

// a node which will be part of the game tree, main pieces of info include: state (board configuration) & associated value
struct Node {
    Node** children;
    int child_count;
    std::vector<std::vector<int>> move_list;
    char state[8][8];
    int val;
};

// method used to initialize a game tree (called everytime the AI has a turn)
Node* CreateTree(char board[8][8], int depth, char player) {
    Node* node = new Node();

    // get the appropriate list moves
    node->move_list = (player == 'w') ? getWhiteLegalMoves(board) : getBlackLegalMoves(board);

    // keep a count of children for indexes later on
    node->child_count = node->move_list.size();

    // copy the passed in board state to the state of the current node
    std::memcpy(node->state, board, 8 * 8 * sizeof(char));

    // determine other player's character
    char other_player = (player == 'w') ? 'b' : 'w';

    // only create children if we're not too deep and this node should have children
    if (depth > 0 && node->child_count > 0) {
        // create an array of nodes as the children of the current node
        node->children = new Node * [node->child_count];

        // cycle through the children and create nodes for them
        for (int i = 0; i < node->child_count; ++i) {
            char tmp_board[8][8];
            std::memcpy(tmp_board, board, 8 * 8 * sizeof(char));

            // must make the associating move first so a subtree of 'that' board configuration can be created
            makeMove(tmp_board, node->move_list[i][0], node->move_list[i][1], player);

            // turn the child into a tree itself
            node->children[i] = CreateTree(tmp_board, depth - 1, other_player);
        }
    }
    else {
        node->children = NULL;
    }

    return node;
}

// crucial minimax method for making smart AI choices (other methods may be added in the future)
int minimax(Node* position, int depth, int alpha, int beta, bool maximizing_player) {
    // if we're at the final layer or this state is a dead sate, return static heurstic
    if (depth == 0 || isGameOver(position->state)) {
        //std::cout<< "returning heursitic: " << heuristic(position->state) << '\n';
        return heuristic(position->state);
    }

    // if maximizing layer...
    if (maximizing_player) {
        int max_eval = -9999999; // set max to worst case

        // for all of the children nodes, recursively call minimax
        // decrease the depth parameter with each call, so we can guarantee we will get to the base case above
        for (int i = 0; i < position->child_count; ++i) {
            int eval = minimax(position->children[i], depth - 1, alpha, beta, false);
            max_eval = std::max(max_eval, eval); // update max if evaluation is >

            //update alpha appropriately, and check for eligibility of alpha prune
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                if (DEBUG_MODE) {
                    std::cout << "DEBUG: PRUNED " << (position->child_count - (i + 1)) << " children.\n";
                }
                break;
            }
        }
        position->val = max_eval; // store the max_eval in this node
        return max_eval;
    }
    else { // minimizing layer...
        int min_eval = 9999999; // set min to worst case
        for (int i = 0; i < position->child_count; ++i) {
            int eval = minimax(position->children[i], depth - 1, alpha, beta, true);
            min_eval = std::min(min_eval, eval); // update min if evaluation is <

            // update beta appropriately, and check for eligibility of beta prune
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        position->val = min_eval; // store min_eval in this node
        return min_eval;
    }
}

// simplified minimax without alpha-beta pruning, similar to above
int minimax(Node* position, int depth, bool maximizing_player) {
    //std::cout << "DEPTH = " << depth << '\n';
    if (depth == 0 || isGameOver(position->state)) {
        //std::cout<< "returning heursitic: " << heuristic(position->state) << '\n';
        return heuristic(position->state);
    }

    if (maximizing_player) {
        int max_eval = -9999999;
        for (int i = 0; i < position->child_count; ++i) {
            int eval = minimax(position->children[i], depth - 1, false);
            max_eval = std::max(max_eval, eval);
        }
        position->val = max_eval;
        return max_eval;
    }
    else {
        int min_eval = 9999999;
        for (int i = 0; i < position->child_count; ++i) {
            int eval = minimax(position->children[i], depth - 1, true);
            min_eval = std::min(min_eval, eval);
        }
        position->val = min_eval;
        return min_eval;
    }
}

// 메인함수
int main() {
    std::cout << "오델로 CLI 프로그램입니다.\n";

    //**** Initialize Game Board *********
    char board[8][8];
    for (auto& i : board) {
        for (char& j : i) {
            j = '-';
        }
    }

    board[3][3] = 'w'; board[3][4] = 'b';
    board[4][3] = 'b'; board[4][4] = 'w';
    //************************************

    int total_moves = 0;
    char player = 'b'; // black always goes first
    std::regex move_input_pattern("[0-7] [0-7]"); // regex for row/col input

    // AI 모드
    if (PLAY_AI) {
        std::regex player_selection_pattern("w|b");
        std::cout << "흰색 또는 검은색 돌을 선택해주세요 (w, b) : ";
        std::string selected_player;
        while (true)
        {
            getline(std::cin, selected_player);
            if (!std::regex_match(selected_player, player_selection_pattern)) {
                std::cout << "\error : b 또는 w를 선택해주세요.\n";
                continue;
            }
            break;
        }

        char player_char = selected_player[0];
        std::cout << "당신이 선택한 돌의 색은 " << ((player_char == 'w') ? "흰색 입니다." : "검은색 입니다") << "!\n\n";

        char ai_char = ((player_char == 'w') ? 'b' : 'w');

        // 게임 시작
        while (!isGameOver(board)) {
            // 현재 플레이어의 move_list를 계산한다.
            std::vector<std::vector<int>> move_list = calculateLegalMoves(board, player);

            // 플레이어가 갈 수 있는 곳이 없는지 체크하기
            // 플레이어 b가 갈 수 있는 곳이 없으면 continue
            if (player == 'b' && getBlackLegalMoves(board).empty()) {
                std::cout << "검은색 이 갈 수 있는 곳이 없습니다.\n";
                player = 'w';
                continue;
            }

            // 플레이어 w가 갈 수 있는 곳이 없으면 continue
            if (player == 'w' && getWhiteLegalMoves(board).empty()) {
                std::cout << "흰색 이 갈 수 있는 곳이 없습니다.\n";
                player = 'b';
                continue;
            }
            //*********************************************************

            int white_total = getScore(board, 'w');
            int black_total = getScore(board, 'b');

            std::cout << "검은색 개수 : " << black_total << std::endl;
            std::cout << "흰색 개수 : " << white_total << std::endl << std::endl;

            std::cout << board; // show board
            std::cout << '\n';
            if (player == player_char) {
                // 움직일 수 있는 곳을 출력
                printLegalMoves(board, player_char);

                std::string user_input;
                // 움직일 수 있는 곳을 입력할 때까지 입력을 받는다.  ( y x 형태로 입력 받는다.)
                while (true) {
                    std::cout << ((player == 'w') ? "(흰색) 돌을 놓을 자리를 입력해주세요 : " : "(검은색) 돌을 놓을 자리를 입력해주세요 : ");
                    std::getline(std::cin, user_input);

                    if (!std::regex_match(user_input, move_input_pattern)) {
                        std::cout << "\nerror : y x 형태로 입력해야 합니다. (0 ~ n)\n";
                        continue;
                    }
                    else {
                        // 문자열 좌표를 숫자로 변환한다.
                        int row = user_input[0] - '0';
                        int col = user_input[2] - '0';

                        try {
                            if (isLegalMove(board, move_list, row, col, player)) {
                                makeMove(board, row, col, player);
                            }
                            else {
                                std::cout << "비정상적인 움직임입니다. 다시 시도해주세요.\n";
                                continue;
                            }
                        }
                        catch (std::range_error& e) {
                            std::cout << e.what() << " - board 범위 바깥으로 입력할 수 없습니다.";
                            return 1;
                        }
                        break;
                    }

                }
            }
            else { 
                auto gametree = CreateTree(board, MINIMAX_DEPTH, player); // game tree representing MINIMAX_DEPTH decisions
                bool maximizer = (player == 'b') ? true : false;

                int optimial_val = minimax(gametree, MINIMAX_DEPTH, -99999999, 99999999, maximizer);

                if (DEBUG_MODE) {
                    std::cout << "DEBUG: AI considered " << gametree->child_count << " initial moves for this board configuration.\n";
                    printLegalMoves(gametree->move_list);
                    for (int i = 0; i < gametree->child_count; ++i) {
                        std::cout << "\t" << i << "th node's heuristic value = " << gametree->children[i]->val << '\n';
                    }
                    std::cout << '\n';
                }

                for (int i = 0; i < gametree->child_count; ++i) {
                    if (gametree->children[i]->val == optimial_val) {
                        bool same_config = true;
                        for (int j = 0; j < 7; ++j) {
                            for (int k = 0; k < 7; ++k) {
                                if (gametree->children[i]->state[j][k] != board[j][k])
                                    same_config = false;
                            }
                        }
                        
                        if (!same_config)
                            std::memcpy(board, gametree->children[i]->state, 8 * 8 * sizeof(char));
                        else {
                            makeMove(board, move_list[0][0], move_list[0][1], player);
                        }
                        break;
                    }
                }
            }

            total_moves += 1;
            //std::cout << '\n' << gb; // Show board

            // Switch players
            player = (player == 'w') ? 'b' : 'w';

        }

    }
    else { // Player2 모드
        while (!isGameOver(board)) {
            std::vector<std::vector<int>> move_list = calculateLegalMoves(board, player);

            std::cout << board;

            std::cout << ((player == 'w') ? "(흰색) 움직일 수 있는 곳\n" : "(검은색) 움직일 수 있는 곳\n");
            printLegalMoves(move_list);

            if (player == 'b' && getBlackLegalMoves(board).size() == 0) {
                player = 'w';
                continue;
            }

            if (player == 'w' && getWhiteLegalMoves(board).size() == 0) {
                player = 'b';
                continue;
            }

            // Print input prompt
            std::cout << ((player == 'w') ? "흰색 : " : "검은색 : ");

            std::string user_input;
            std::getline(std::cin, user_input);

            if (!std::regex_match(user_input, move_input_pattern)) {
                std::cout << "\nerror : y x 형태로 입력해야 합니다. (1 ~ n)\n";
                continue;
            }

            int row = user_input[0] - '0';
            int col = user_input[2] - '0';

            try {
                if (isLegalMove(board, move_list, row, col, player)) {
                    makeMove(board, row, col, player);
                }
                else {
                    std::cout << "비정상적인 움직임입니다. 다시 시도해주세요.\n";
                    continue;
                }
            }
            catch (std::range_error& e) {
                std::cout << e.what() << " - board 범위 바깥으로 입력할 수 없습니다.";
                return 1;
            }

            total_moves += 1;
            int white_total = getScore(board, 'w');
            int black_total = getScore(board, 'b');

            std::cout << "\n검은색   총점 : " << black_total << '\n';
            std::cout << "흰색     총점 : " << white_total << "\n";

            player = (player == 'w') ? 'b' : 'w';
        }
    }

    std::cout << board;
    printWinner(board);
    return 0;
}
