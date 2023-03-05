#include "../include/board.h"

Board::Board(const Referee& referee, const std::string& difficulty)
    : m_referee(referee)
{
    m_dimension = difficulty == "easy" ? 8 : 10;
}


void Board::initialize_spaces() {

    for (int i = 0; i < m_dimension; i++) {

        m_board.emplace_back();

        for (int j = 0; j < m_dimension; j++) {

            m_board[i].emplace_back("*");
        }
    }
}


void Board::place_stone(const int x, const int y) {

    if (m_referee.get_current_player() == "Player 1") {

        m_board[x][y] = m_p1_stone;
    }

    else {

        m_board[x][y] = m_p2_stone;
    }
}


void Board::prompt() {

    int x = 0;
    int y = 0;

    while (m_referee.has_vacancy(m_board)) {

        // Human's turn
        if (m_referee.get_current_player() == "Player 1") {

            do {

                // If a character is entered for some reason (prevents infinite loop)
                if (std::cin.fail()) {

                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                clear_screen();
                std::cout << *this << "\n";

                m_referee.print_scoreboard();

                std::cout << m_referee.get_current_player()
                          << ", enter valid coordinates (x, y): ";

                std::cin >> x >> y;
            }
            while (!m_referee.validate_move(m_board, x, y) || std::cin.fail());
        }

        // AI's turn
        else {

            AI ai(m_board, m_referee);
            std::array<int, 2> ai_move = ai.get_ai_move();

            x = ai_move[0];
            y = ai_move[1];
        }

        place_stone(x, y);
        m_referee.set_last_placed(x, y);

        m_referee.update_player_scores(m_board);
        m_referee.switch_player();
    }

    m_referee.declare_winner();
}


void Board::clear_screen() {

    #if defined _WIN32

        // Retrieved from https://stackoverflow.com/a/6487534

        COORD top_left  = {0, 0};
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screen;
        DWORD written;

        GetConsoleScreenBufferInfo(console, &screen);

        FillConsoleOutputCharacterA(
            console, ' ', screen.dwSize.X * screen.dwSize.Y, top_left, &written
        );

        FillConsoleOutputAttribute(
            console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            screen.dwSize.X * screen.dwSize.Y, top_left, &written
        );

        SetConsoleCursorPosition(console, top_left);

    #else

        // Retrieved from https://stackoverflow.com/a/32008479
        std::cout << "\033[2J\033[1;1H";

    #endif
}


std::ostream& operator<<(std::ostream& out, const Board& b) {

    out << "  ";

    for (int i = 0; i < b.m_dimension; i++) {

        out << i << " ";
    }

    out << "\n";

    for (int i = 0; i < b.m_dimension; i++) {

        out << i << " ";

        for (int j = 0; j < b.m_dimension; j++) {

            out << b.m_board[i][j] << " ";
        }

        out << "\n";
    }

    return out;
}
