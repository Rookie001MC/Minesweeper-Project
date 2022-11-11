#include <chrono>
#include <iostream>
#include <tuple>
#include <vector>

#include "minesweeper.hpp"
#include "util_functions.cpp"

std::vector<std::vector<MinesweeperCell>> create_game_field(int rows, int cols,
    int mines);
int random_num_gen(int from, int to);
void print_help_sel();
std::tuple<int, int, int> difficulty();
int main(void)
{

    std::tuple<int, int, int> game_data = difficulty();

    int x = std::get<0>(game_data);
    int y = std::get<1>(game_data);
    int mines = std::get<2>(game_data);

    // Take in the user input of X, Y and mine numbers
    // Creates the solution	field

    std::vector<std::vector<MinesweeperCell>> solution_field = create_game_field(x, y, mines);
    // Creates the game field
    std::vector<std::vector<MinesweeperCell>> game_field;
}

std::vector<std::vector<MinesweeperCell>> create_game_field(int rows, int cols,
    int mines)
{
    /**
     * Creates the `board`, which contains the cell data and mine positions,
     *
     * @param rows Number of rows
     * @param cols Number of mines
     * @param mines Number of mines
     *
     * @return still have no idea yet
     */

    // Creates the solution field
    std::vector<std::vector<MinesweeperCell>> table(
        rows, std::vector<MinesweeperCell>(cols));

    std::cout << "\nCreating a hypothetical Minesweeper field with " << rows
              << "x" << cols << ":\n";
    // Fill in the board with MinesweeperCells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            table[i][j] = MinesweeperCell();
        }
    }

    // Generate the mine positions:
    for (int current_mine = 0; current_mine < mines; current_mine++) {
        int random_bomb_row = random_num_gen(0, rows);
        int random_bomb_col = random_num_gen(0, cols);

        // Check if the selected position has a mine already
        if (table[random_bomb_row][random_bomb_col].mine == true) {
            continue;
        } else { // Set the randomized position to be a mine
            table[random_bomb_row][random_bomb_col].mine = true;

            // Find all the neighboring cells of the mine, then add up the neighbor
            // counts for each neighboring cells
            if (random_bomb_row != 0 && random_bomb_col != 0) // check for edge
                table[random_bomb_row - 1][random_bomb_col - 1].neighbors += 1;

            if (random_bomb_row != 0) // check for edge
                table[random_bomb_row - 1][random_bomb_col].neighbors += 1;

            if (random_bomb_row != 0 && random_bomb_col != cols - 1) // check for edge
                table[random_bomb_row - 1][random_bomb_col + 1].neighbors += 1;

            if (random_bomb_col != 0) // check for edge
                table[random_bomb_row][random_bomb_col - 1].neighbors += 1;

            if (random_bomb_col != cols - 1) // check for edge
                table[random_bomb_row][random_bomb_col + 1].neighbors += 1;

            if (random_bomb_row != rows - 1 && random_bomb_col != 0) // check for edge
                table[random_bomb_row + 1][random_bomb_col - 1].neighbors += 1;

            if (random_bomb_row != rows - 1) // check for edge
                table[random_bomb_row + 1][random_bomb_col].neighbors += 1;

            if (random_bomb_row != rows - 1 && random_bomb_col != cols - 1) // check for edge
                table[random_bomb_row + 1][random_bomb_col + 1].neighbors += 1;
        }
    }

    // Debug code for visualising game cells
    std::cout << "Solution table: \n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (table[i][j].mine == true) {
                std::cout << '*';
            } else {
                if (table[i][j].neighbors != 0) {
                    std::cout << table[i][j].neighbors;
                } else {
                    std::cout << '.';
                }
            }
        }
        std::cout << "\n";
    }
    return table;
}
