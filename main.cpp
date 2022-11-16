#include <iostream>
#include <tuple>
#include <vector>

#include "util_functions.cpp"

std::vector<std::vector<MinesweeperCell>> create_game_field(int rows, int cols,
                                                            int mines);
void game_logic(std::vector<std::vector<MinesweeperCell>> game_table);
int random_num_gen(int from, int to);
void print_help_sel();
std::tuple<int, int, int> difficulty();
void start_game();
void print_current_game_table(
    std::vector<std::vector<MinesweeperCell>>& game_table, int rows, int cols);
void make_move(std::vector<std::vector<MinesweeperCell>>& game_table, int rows,
               int cols);
void reveal_cell(std::vector<std::vector<MinesweeperCell>>& game_table,
                 int rows, int cols);
void flag_cell(std::vector<std::vector<MinesweeperCell>>& game_table, int rows,
               int cols);

void clear_screen();
void sleep(int milliseconds);
int main(void) { start_game(); }
void start_game()
{
    auto [rows, cols, mines] = difficulty();

    std::vector<std::vector<MinesweeperCell>> game_field =
        create_game_field(rows, cols, mines);
    game_logic(game_field);
}

void game_logic(std::vector<std::vector<MinesweeperCell>> game_table)
{
    /**
     * Handles the game logic
     *
     * @param game_table the game table itself.
     */

    bool game_over = false;

    int move_count = 0;

    const int rows = game_table.size();
    const int cols = game_table[0].size();

    while (game_over == false) {
        print_current_game_table(game_table, rows, cols);
        make_move(game_table, rows, cols);
        // Forcing the game to actually end prematurely, for debugging
        // game_over = true;
    }
}

void print_current_game_table(
    std::vector<std::vector<MinesweeperCell>>& game_table, int rows, int cols)
{
    /**
     * Prints the current game table.
     */
    // Characters to be used for printing to console
    const char UNREVEALED = '.';
    const char MINE = '*';
    const char FLAGGED = 'F';
    const char UNKNOWN = '?';

    // Printing logic
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (game_table[i][j].revealed == true) {
                if (game_table[i][j].mine == true) {
                    std::cout << MINE;
                }
                else {
                    std::cout << game_table[i][j].neighbors;
                }
            }
            else {
                if (game_table[i][j].flagged == flag_status::UNSURE) {
                    std::cout << UNKNOWN;
                }
                else if (game_table[i][j].flagged == flag_status::FLAGGED) {
                    std::cout << FLAGGED;
                }
                else {
                    std::cout << UNREVEALED;
                }
            }
        }
        std::cout << "\n";
    }
}

void make_move(std::vector<std::vector<MinesweeperCell>>& game_table, int rows, int cols)
{
    char selection;
    std::cout << "Make a selection: \n";
    std::cout << "R - Reveal\nF - Flag\nN - New Game\nQ - Save and Quit Game\n";
    std::cout << "> ";
    std::cin >> selection;

    if (selection == 'R' || selection == 'r') {
        reveal_cell(game_table, rows, cols);
    }
    else if (selection == 'F' || selection == 'f') {
        flag_cell(game_table, rows, cols);
    }
    else {
        std::cout
            << "This feature is either not implemented yet, or the selection "
               "is invalid!";
    }
    sleep(500);
    clear_screen();
}

void reveal_cell(std::vector<std::vector<MinesweeperCell>>& game_table,
                 int rows, int cols)
{
    auto [row, col] = ask_position();
    if (row > rows || row <= 0 || col > cols || col <= 0) {
        std::cout << "Your selection is out of bounds!\n";
    }
    else if (game_table[row][col].revealed == true ||
             game_table[row][col].flagged == FLAGGED ||
             game_table[row][col].flagged == UNSURE) {
        std::cout << "This cell cannot be opened!\n";
    }
    else {
        game_table[row][col].revealed = true;
    }
    return;
}

void flag_cell(std::vector<std::vector<MinesweeperCell>>& game_table, int rows,
               int cols)
{
    auto [row, col] = ask_position();
    if (row > rows || row <= 0 || col > cols || col <= 0) {
        std::cout << "Your selection is out of bounds!\n";
    }
    else if (game_table[row][col].revealed == true) {
        std::cout << "This cell cannot be flagged!\n";
    }
    else {
        if (game_table[row][col].flagged == flag_status::UNFLAGGED) {
            game_table[row][col].flagged = flag_status::FLAGGED;
        }
        else if (game_table[row][col].flagged == flag_status::FLAGGED) {
            game_table[row][col].flagged = flag_status::UNSURE;
        }
        else if (game_table[row][col].flagged == flag_status::UNSURE) {
            game_table[row][col].flagged = flag_status::UNFLAGGED;
        }
    }
    return;
}