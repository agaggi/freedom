#pragma once

#include <iostream>
#include <vector>
#include <limits>

#if defined _WIN32
    #include <windows.h>
#endif

#include "referee.h"
#include "ai.h"

using grid = std::vector<std::vector<std::string>>;

class Board {

    public:

        /**
         * @brief Constructor for the Board class.
         *
         * @param referee The referee object used for the entire program
         * @param difficulty The difficulty entered at the command-line
         */
        Board(const Referee& referee, const std::string& difficulty);

        /**
         * @brief Initializes the game board with empty spaces.
         *
         * Easy - 8x8 board
         * Normal - 10x10 board
         */
        void initialize_spaces();

        /**
         * @brief Places the current player's stone on the specified location.
         *
         * @param x The x-coordinate specified
         * @param y The y-coordinate specified
         */
        void place_stone(const int x, const int y);

        /**
         * @brief The "runner" function for the application.
         *
         * Incorperates many functions together to present the gameboard to the terminal.
         */
        void prompt();

        /**
         * Clears the terminal window
         */
        void clear_screen();

        /**
         * @brief Overloads the << operator for the Board class.
         *
         * Clears the screen and outputs a vector containing the spaces on
         * the board in a nxn orientation
         *
         * @param out Stream object used for output
         * @param b Board object to have its attributes outputted
         * @return The stream object
         */
        friend std::ostream& operator<<(std::ostream& out, const Board& b);

    private:

        // Set by the constructor
        int m_dimension;

        grid m_board;
        Referee m_referee;

        const std::string m_p1_stone = "\u25CF";    // White circle
        const std::string m_p2_stone = "\u25CB";    // Black circle
};
