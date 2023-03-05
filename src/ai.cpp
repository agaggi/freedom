#include "../include/ai.h"

AI::BoardState::BoardState(const grid& board, const std::array<int, 2>& last_placed)
    : board(board), last_placed(last_placed)
{
}


AI::AI(const grid& board, const Referee& referee)
    : m_current_state(BoardState(board, referee.get_last_placed())), m_referee(referee)
{
}


std::array<int, 2> AI::get_ai_move() {

    // Value for depth represents how many moves we look ahead
    std::array<int, 3> ai_move = minimax(m_current_state, 6, INT32_MIN, INT32_MAX, true);

    return std::array<int, 2> {ai_move[1], ai_move[2]};
}


std::array<int, 3> AI::retrieve_board_value(const BoardState& state) {

    m_referee.update_player_scores(state.board);

    const std::array<int, 2> scores = m_referee.get_player_scores();
    const int state_value = scores[1] - scores[0];

    return std::array<int, 3> {state_value, state.last_placed[0], state.last_placed[1]};
}


std::vector<AI::BoardState> AI::make_possible_moves(const BoardState& state,
                                                    const coordinates& coords,
                                                    const std::string& stone) {

    std::vector<BoardState> possible_moves;

    for (const std::array<int, 2>& coord : coords) {

        // If the move is legal, make a new state
        if (state.board[coord[0]][coord[1]] == "*") {

            // Deepcopy
            grid board_copy = grid(state.board);
            board_copy[coord[0]][coord[1]] = stone;

            possible_moves.emplace_back(board_copy, coord);
        }
    }

    return possible_moves;
}


std::vector<AI::BoardState> AI::generate_moves(const BoardState& state,
                                               const std::string& stone) {

    const int prev_x = state.last_placed[0];
    const int prev_y = state.last_placed[1];

    coordinates coords;

    // Top-left corner
    if (prev_x == 0 && prev_y == 0) {

        coords.push_back({prev_x+1, prev_y});
        coords.push_back({prev_x, prev_y+1});
    }

    // Top-right corner
    else if (prev_x == 0 && prev_y == state.board.size() - 1) {

        coords.push_back({prev_x+1, prev_y});
        coords.push_back({prev_x, prev_y-1});
    }

    // Bottom-left corner
    else if (prev_x == state.board.size() - 1 && prev_y == 0) {

        coords.push_back({prev_x-1, prev_y});
        coords.push_back({prev_x, prev_y+1});
    }

    // Bottom-right corner
    else if (prev_x == state.board.size() - 1 && prev_y == state.board.size() - 1) {

        coords.push_back({prev_x-1, prev_y});
        coords.push_back({prev_x, prev_y-1});
    }

    // First row
    else if (prev_x == 0) {

        coords.push_back({prev_x+1, prev_y});
        coords.push_back({prev_x, prev_y+1});
        coords.push_back({prev_x, prev_y-1});
    }

    // Last row
    else if (prev_x == state.board.size() - 1) {

        coords.push_back({prev_x-1, prev_y});
        coords.push_back({prev_x, prev_y+1});
        coords.push_back({prev_x, prev_y-1});
    }

    // First column
    else if (prev_y == 0) {

        coords.push_back({prev_x+1, prev_y});
        coords.push_back({prev_x-1, prev_y});
        coords.push_back({prev_x, prev_y+1});
    }

    // Last column
    else if (prev_y == state.board.size() - 1) {

        coords.push_back({prev_x+1, prev_y});
        coords.push_back({prev_x-1, prev_y});
        coords.push_back({prev_x, prev_y-1});
    }

    // Middle portion of the board
    else {

        coords.push_back({prev_x+1, prev_y});
        coords.push_back({prev_x-1, prev_y});
        coords.push_back({prev_x, prev_y+1});
        coords.push_back({prev_x, prev_y-1});
    }

    std::vector<BoardState> possible_moves = make_possible_moves(state, coords, stone);

    // Freedom mechanic
    if (possible_moves.empty()) {

        coords.erase(coords.begin(), coords.end());

        for (int i = 0; i < state.board.size(); i++) {

            for (int j = 0; j < state.board.size(); j++) {

                if (state.board[i][j] == "*") {

                    coords.push_back({i, j});
                }
            }
        }

        possible_moves = make_possible_moves(state, coords, stone);
    }

    return possible_moves;
}


std::array<int, 3> AI::minimax(const BoardState& state, int depth, int alpha,
                               int beta, bool ai_turn) {

    // Once we reach the number of states we are looking ahead
    // or if the game is finished, begin the algorithm
    if (depth == 0 || !m_referee.has_vacancy(state.board)) {

        return retrieve_board_value(state);
    }

    int x = 0;
    int y = 0;

    std::string stone = ai_turn ? "\u25CB" : "\u25CF";

    if (ai_turn) {

        int max_value = INT32_MIN;
        std::vector<BoardState> possible_moves = generate_moves(state, stone);

        for (const BoardState& new_state : possible_moves) {

            // board value, last placed x, last placed y
            std::array<int, 3> node = minimax(new_state, depth-1, alpha, beta, false);

            if (node[0] > max_value) {

                max_value = node[0];

                x = new_state.last_placed[0];
                y = new_state.last_placed[1];
            }

            alpha = std::max(alpha, node[0]);

            // Pruning
            if (beta <= alpha) {

                break;
            }
        }

        return std::array<int, 3> {max_value, x, y};
    }

    // If the algorithm is calculating a human player's move
    int min_value = INT32_MAX;
    std::vector<BoardState> possible_moves = generate_moves(state, stone);

    for (const BoardState& new_state : possible_moves) {

        // board value, last placed x, last placed y
        std::array<int, 3> node = minimax(new_state, depth-1, alpha, beta, true);

        if (node[0] < min_value) {

            min_value = node[0];

            x = new_state.last_placed[0];
            y = new_state.last_placed[1];
        }

        beta = std::min(beta, node[0]);

        // Pruning
        if (beta <= alpha) {

            break;
        }
    }

    return std::array<int, 3> {min_value, x, y};
}
