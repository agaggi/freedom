#include "../include/referee.h"

void Referee::switch_player() {

    if (m_current_player == "Player 1") {

        m_current_player = "Player 2";
    }

    else {

        m_current_player = "Player 1";
    }

    m_move++;
}


bool Referee::has_vacancy(const grid& board) {

    int vacant_spaces = 0;

    for (int i = 0; i < board.size(); i++) {

        for (int j = 0; j < board.size(); j++) {

            if (board[i][j] == "*") {

                vacant_spaces++;
            }
        }
    }

    return vacant_spaces != 0;
}


void Referee::declare_winner() {

    if (m_p1_score > m_p2_score) {

        std::cout << "Player 1 wins!\n";
    }

    else if (m_p1_score < m_p2_score) {

        std::cout << "Player 2 wins!\n";
    }

    else {

        std::cout << "It's a draw.\n";
    }
}


void Referee::update_player_scores(const grid& board) {

    // Recalculate a player's overall score per function call
    m_p1_score = 0;
    m_p2_score = 0;

    // Pass in horizontal portions of the board
    for (const std::vector<std::string>& row : board) {

        calculate_player_scores(row);
    }

    // Pass in vertical portions of the board
    for (int i = 0; i < board.size(); i++) {

        std::vector<std::string> column;
        column.reserve(board.size() - 1);

        for (int j = 0; j < board.size(); j++) {

            column.emplace_back(board[j][i]);
        }

        calculate_player_scores(column);
    }

    // Pass in diagonal portions of the board
    int n = 0;

    std::vector<std::string> main_forward_diagonal;
    std::vector<std::string> main_backward_diagonal;

    grid upper_forward_diagonals;
    grid lower_forward_diagonals;
    grid upper_backward_diagonals;
    grid lower_backward_diagonals;

    for (int i = 0; i < board.size(); i++) {

        n++;

        if (i < board.size() - 1) {

            upper_forward_diagonals.emplace_back();
            lower_forward_diagonals.emplace_back();
            upper_backward_diagonals.emplace_back();
            lower_backward_diagonals.emplace_back();
        }

        for (int j = 0; j < board.size(); j++) {

            if (i == j) {

                main_forward_diagonal.emplace_back(board[i][j]);
            }

            if (i + j == board.size() - 1) {

                main_backward_diagonal.emplace_back(board[i][j]);
            }

            for (int k = 0; k < board.size(); k++) {

                if (j + n == k) {

                    upper_forward_diagonals[i].emplace_back(board[j][k]);
                }

                else if (j - n == k) {

                    lower_forward_diagonals[i].emplace_back(board[j][k]);
                }

                else if (j + k + n == board.size() - 1) {

                    upper_backward_diagonals[i].emplace_back(board[j][k]);
                }

                else if (j + k - n == board.size() - 1) {

                    lower_backward_diagonals[i].emplace_back(board[j][k]);
                }
            }
        }
    }

    calculate_player_scores(main_forward_diagonal);
    calculate_player_scores(main_backward_diagonal);

    // Because the board is nxn, each upper/lower diagonal vectors will have
    // the same amount of entries
    for (int i = 0; i < upper_forward_diagonals.size(); i++) {

        calculate_player_scores(upper_forward_diagonals[i]);
        calculate_player_scores(lower_forward_diagonals[i]);
        calculate_player_scores(upper_backward_diagonals[i]);
        calculate_player_scores(lower_backward_diagonals[i]);
    }
}


void Referee::calculate_player_scores(const std::vector<std::string>& slice) {

    const std::string white_stone = "\u25CF";
    const std::string black_stone = "\u25CB";

    int white_stone_count = 0;
    int black_stone_count = 0;

    for (int i = 0; i < slice.size(); i++) {

        if (slice[i] == white_stone) {

            white_stone_count++;
            black_stone_count = 0;

            if (white_stone_count == 4) {

                m_p1_score++;
            }

            if (white_stone_count == 5) {

                m_p1_score--;
            }
        }

        else if (slice[i] == black_stone) {

            white_stone_count = 0;
            black_stone_count++;

            if (black_stone_count == 4) {

                m_p2_score++;
            }

            if (black_stone_count == 5) {

                m_p2_score--;
            }
        }

        // If a '*' interupts the sequence
        else {

            white_stone_count = 0;
            black_stone_count = 0;
        }
    }
}


bool Referee::validate_move(const grid& board, const int new_x, const int new_y) {

    // Check for out-of-bounds
    if (new_x < 0 || new_y < 0 || new_x > board.size() - 1 || new_y > board.size() - 1) {

        return false;
    }

    // Check if the selected coordinate is vacant
    if (board[new_x][new_y] != "*") {

        return false;
    }

    // First move only - place anywhere
    if (m_move == 0) {

        return true;
    }

    const int prev_x = m_last_placed[0];
    const int prev_y = m_last_placed[1];

    // First row - don't check above
    if (prev_x == 0) {

        // Top-left corner - don't check left either
        if (prev_y == 0) {

            // If the piece was placed right or below the last placed piece
            if ((new_x == prev_x+1 && new_y == prev_y) ||
                (new_x == prev_x && new_y == prev_y+1)) {

                return true;
            }

            // Freedom check
            else if (board[prev_x+1][prev_y] != "*" && board[prev_x][prev_y+1] != "*") {

                return true;
            }
        }

        // Top-right corner - don't check right either
        else if (prev_y == board.size() - 1) {

            // If the piece was placed left or below the last placed piece
            if ((new_x == prev_x+1 && new_y == prev_y) ||
                (new_x == prev_x && new_y == prev_y-1)) {

                return true;
            }

            // Freedom check
            else if (board[prev_x+1][prev_y] != "*" && board[prev_x][prev_y-1] != "*") {

                return true;
            }
        }

        // Middle portion of top-row - check left, right, and below
        else if ((new_x == prev_x+1 && new_y == prev_y) ||
                 (new_x == prev_x && new_y == prev_y+1) ||
                 (new_x == prev_x && new_y == prev_y-1)) {

            return true;
        }

        // Freedom check
        else if (board[prev_x+1][prev_y] != "*" &&
                 board[prev_x][prev_y+1] != "*" &&
                 board[prev_x][prev_y-1] != "*") {

            return true;
        }
    }

    // Last row - don't check below
    else if (prev_x == board.size() - 1) {

        // Bottom-left corner - don't check left either
        if (prev_y == 0) {

            // If the piece was placed right or above the last placed piece
            if ((new_x == prev_x-1 && new_y == prev_y) ||
                (new_x == prev_x && new_y == prev_y+1)) {

                return true;
            }

            // Freedom check
            else if (board[prev_x-1][prev_y] != "*" && board[prev_x][prev_y+1] != "*") {

                return true;
            }
        }

        // Bottom-right corner - don't check right either
        else if (prev_y == board.size() - 1) {

            // If the piece was placed left or below the last placed piece
            if ((new_x == prev_x-1 && new_y == prev_y) ||
                (new_x == prev_x && new_y == prev_y-1)) {

                return true;
            }

            // Freedom check
            else if (board[prev_x-1][prev_y] != "*" && board[prev_x][prev_y-1] != "*") {

                return true;
            }
        }

        // Middle portion of bottom-row - check left, right, and above
        else if ((new_x == prev_x-1 && new_y == prev_y) ||
                 (new_x == prev_x && new_y == prev_y+1) ||
                 (new_x == prev_x && new_y == prev_y-1)) {

            return true;
        }

        // Freedom check
        else if (board[prev_x-1][prev_y] != "*" &&
                 board[prev_x][prev_y+1] != "*" &&
                 board[prev_x][prev_y-1] != "*") {

            return true;
        }
    }

    // If we are in the first column - check all but left
    // Note: the special cases (i.e. corners) are already checked for above
    else if (prev_y == 0) {

        // If the piece was placed right or below the last placed piece
        if ((new_x == prev_x+1 && new_y == prev_y) ||
            (new_x == prev_x && new_y == prev_y+1) ||
            (new_x == prev_x-1 && new_y == prev_y)) {

            return true;
        }

        // Freedom check
        else if (board[prev_x+1][prev_y] != "*" &&
                 board[prev_x][prev_y+1] != "*" &&
                 board[prev_x-1][prev_y] != "*") {

            return true;
        }
    }

    // If we are in the last column - check all but right
    else if (prev_y == board.size() - 1) {

        // If the piece was placed right or below the last placed piece
        if ((new_x == prev_x+1 && new_y == prev_y) ||
            (new_x == prev_x && new_y == prev_y-1) ||
            (new_x == prev_x-1 && new_y == prev_y)) {

            return true;
        }

        // Freedom check
        else if (board[prev_x+1][prev_y] != "*" &&
                 board[prev_x][prev_y-1] != "*" &&
                 board[prev_x-1][prev_y] != "*") {

            return true;
        }
    }

    // We are in the main "body" of the board - check every adjacent direction
    else if ((new_x == prev_x-1 && new_y == prev_y) ||
             (new_x == prev_x && new_y == prev_y+1) ||
             (new_x == prev_x+1 && new_y == prev_y) ||
             (new_x == prev_x && new_y == prev_y-1)) {

        return true;
    }

    // Freedom check
    else if (board[prev_x+1][prev_y] != "*" &&
             board[prev_x][prev_y+1] != "*" &&
             board[prev_x-1][prev_y] != "*" &&
             board[prev_x][prev_y-1] != "*") {

        return true;
    }

    return false;
}
