#include "./include/board.h"
#include "./include/referee.h"

/**
 * @brief Main function for the program.
 *
 * Takes in the command-line argument entered for difficulty and checks
 * if it is valid.
 *
 * @param argc The number of arguments entered
 * @param argv The provided difficulty
 */
int main(int argc, char** argv) {

    // If no argument provided, set to empty string to prevent errors
    const std::string difficulty = argv[1] ? argv[1] : "";

    if (argc < 2) {

        std::cout << "[Error] Not enough arguments entered.\n";
    }

    else if (difficulty != "easy" && difficulty != "normal") {

        std::cout << "[Error] Invalid difficulty entered. Valid arguments are:\n\n"
                  << "  easy    ->  8x8 board\n"
                  << "  normal  ->  10x10 board\n";
    }

    else {

        Referee referee;
        Board board(referee, difficulty);

        board.initialize_spaces();
        board.prompt();
    }
}
