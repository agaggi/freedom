#pragma once

#include <iostream>
#include <vector>
#include <array>

using grid = std::vector<std::vector<std::string>>;

class Referee {

    public:

        /**
         * @brief Enables turn-taking.
         */
        void switch_player();

        /**
         * @brief Prints the scoreboard, showing player scores.
         */
        void print_scoreboard();

        /**
         * @brief Checks if there are vacant spaces on the game board.
         * @return (T/F) If there are any vanacnt spaces
         */
        bool has_vacancy(const grid& board);

        /**
         * @brief Declares the winner based on score.
         *
         * Checks which player has the higher score. If players have the same
         * score then it is announced as a draw.
         */
        void declare_winner();

        /**
         * @brief Provides the most recent scores of players based on a board state.
         *
         * The passed in board state is evaluated horizontally, vertically, and
         * diagonally to see if a player has 4 of their stones in succession.
         *
         * @param board The board state to be evaluated
         */
        void update_player_scores(const grid& board);

        /**
         * @brief Determines if a player has gained points based off a section of the
         * board.
         *
         * A player gains points by having <b>4</b> of their stones in an uninterupted
         * sequence and can be in any direction.
         *
         * If a player has more than 4 stones in a row, then they lose a point.
         *
         * @param slice A 1D "slice" of the board state (horizontal, vertical, or
         * diagonal)
         */
        void calculate_player_scores(const std::vector<std::string>& slice);

        /**
         * @brief Logic for what determines a valid move.
         *
         * Performs various checks on the user's input, such as bounds checking,
         * referencing the last placed stone, and the freedom mechanic.
         *
         * If the last placed piece is surrounded, then the player has freedom, meaning
         * they can place their next stone anywhere that is vacant.
         *
         * @param board The current board state
         * @param x The x-coordinate entered
         * @param y The y-coordinate entered
         *
         * @return (T/F) Whether the move was valid or not
         */
        bool validate_move(const grid& board, const int x, const int y);

        /**
         * @brief Getter method to receive player scores
         * @return The player scores in a single array
         */
        const std::array<int, 2> get_player_scores() const;

        /**
         * @brief Getter method for current player.
         * @return The current player
         */
        const std::string get_current_player() const { return m_current_player; }

        /**
         * @brief Getter method for the last placed stone
         * @return The array containing the coordinate of the last placed stone
         */
        const std::array<int, 2> get_last_placed() const { return m_last_placed; }

        /**
         * @brief Setter method for the last placed stone.
         *
         * @param x The x-coordinate entered
         * @param y The y-coordinate entered
         */
        void set_last_placed(const int x, const int y);


    private:

        std::string m_current_player = "Player 1";
        int m_move = 0;

        std::array<int, 2> m_last_placed = {0};

        int m_p1_score = 0;
        int m_p2_score = 0;
};


inline void Referee::print_scoreboard() {

    std::cout << "Last placed: (" << m_last_placed[0] << ", " << m_last_placed[1] << ")\n\n";

    std::cout << "\u25CF Player 1: " << m_p1_score << "\n"
              << "\u25CB Player 2: " << m_p2_score << "\n\n";
}


inline void Referee::set_last_placed(const int x, const int y) {

    m_last_placed[0] = x;
    m_last_placed[1] = y;
}


inline const std::array<int, 2> Referee::get_player_scores() const {

    return std::array<int, 2> {m_p1_score, m_p2_score};
}
