#include "common.h"
#include "Board.h"

// 메인함수
int main() {
    std::cout << "객체지향설계 텀프로젝트 11조\n";
    std::cout << "오델로 CLI 프로그램입니다.\n";

    // board 초기화
    int n = 8;
    std::cout << "오델로 Board의 크기를 입력해주세요 : ";
    std::string user_input;
    std::getline(std::cin, user_input);
    std::stringstream ssInt(user_input);
    ssInt >> n;

    Board* board = new Board(n);

    int total_moves = 0;
    // 검은돌이 먼저 진행한다.
    char player = 'b';
    std::regex move_input_pattern("[0-7] [0-7]"); // regex for row/col input

    while (!board->isGameOver()) {
        std::vector<std::vector<int>> move_list = board->calculateLegalMoves(player);

        board->printAllBoard();

        std::cout << ((player == 'w') ? "(흰색) 움직일 수 있는 곳\n" : "(검은색) 움직일 수 있는 곳\n");
        board->printLegalMoves(move_list);

        if (player == 'b' && board->getBlackLegalMoves().size() == 0) {
            player = 'w';
            continue;
        }

        if (player == 'w' && board->getWhiteLegalMoves().size() == 0) {
            player = 'b';
            continue;
        }

        // Print input prompt
        std::cout << ((player == 'w') ? "흰색 : " : "검은색 : ");

        std::getline(std::cin, user_input);

        if (!std::regex_match(user_input, move_input_pattern)) {
            std::cout << "\nerror : y x 형태로 입력해야 합니다. (1 ~ n)\n";
            continue;
        }

        int row = user_input[0] - '0';
        int col = user_input[2] - '0';

        try {
            if (board->isLegalMove(move_list, row, col, player)) {
                board->makeMove(row, col, player);
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
        int white_total = board->getScore('w');
        int black_total = board->getScore('b');

        std::cout << "\n검은색   총점 : " << black_total << '\n';
        std::cout << "흰색     총점 : " << white_total << "\n";

        player = (player == 'w') ? 'b' : 'w';
    }

    board->printAllBoard();
    board->printWinner();
    return 0;
}
