#include <chrono>
#include <iostream>
#include <tuple>
#include <vector>

#include "minesweeper.hpp"
#include "util_functions.cpp"

std::vector<std::vector<MinesweeperCell> > create_game_field(int rows, int cols,
                                                             int mines);
void game_logic(std::vector<std::vector<MinesweeperCell> >& game_table);
int random_num_gen(int from, int to);
void print_help_sel();
std::tuple<int, int, int> difficulty();
void start_game();
void print_current_game_table(
    std::vector<std::vector<MinesweeperCell> >& game_table);
void make_move();
int main(void) { start_game(); }

void start_game()
{
  std::tuple<int, int, int> game_data = difficulty();

  int rows = std::get<0>(game_data);
  int cols = std::get<1>(game_data);
  int mines = std::get<2>(game_data);

  std::vector<std::vector<MinesweeperCell> > game_field =
      create_game_field(rows, cols, mines);
  game_logic(game_field);
}

std::vector<std::vector<MinesweeperCell> > create_game_field(int rows, int cols,
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
  std::vector<std::vector<MinesweeperCell> > table(
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
    }
    else {     // Set the randomized position to be a mine
      table[random_bomb_row][random_bomb_col].mine = true;

      // Find all the neighboring cells of the mine, then add up
      // the neighbor counts for each neighboring cells
      if (random_bomb_row != 0 && random_bomb_col != 0)     // check for edge
        table[random_bomb_row - 1][random_bomb_col - 1].neighbors += 1;

      if (random_bomb_row != 0)     // check for edge
        table[random_bomb_row - 1][random_bomb_col].neighbors += 1;

      if (random_bomb_row != 0 &&
          random_bomb_col != cols - 1)     // check for edge
        table[random_bomb_row - 1][random_bomb_col + 1].neighbors += 1;

      if (random_bomb_col != 0)     // check for edge
        table[random_bomb_row][random_bomb_col - 1].neighbors += 1;

      if (random_bomb_col != cols - 1)     // check for edge
        table[random_bomb_row][random_bomb_col + 1].neighbors += 1;

      if (random_bomb_row != rows - 1 &&
          random_bomb_col != 0)     // check for edge
        table[random_bomb_row + 1][random_bomb_col - 1].neighbors += 1;

      if (random_bomb_row != rows - 1)     // check for edge
        table[random_bomb_row + 1][random_bomb_col].neighbors += 1;

      if (random_bomb_row != rows - 1 &&
          random_bomb_col != cols - 1)     // check for edge
        table[random_bomb_row + 1][random_bomb_col + 1].neighbors += 1;
    }
  }

  return table;
}

void game_logic(std::vector<std::vector<MinesweeperCell> >& game_table)
{
  /**
   * Handles the game logic
   *
   * @param game_table the game table itself.
   */

  bool game_over = false;

  int move_count = 0;

  while (game_over == false) {
    print_current_game_table(game_table);
    make_move();
    // Forcing the game to actually end prematurely, for debugging
    game_over = true;
  }
}

void print_current_game_table(
    std::vector<std::vector<MinesweeperCell> >& game_table)
{
  /**
   * Prints the current game table.
   */
  // Characters to be used for printing to console
  const char UNREVEALED = '.';
  const char MINE = '*';
  const char FLAGGED = 'F';
  const char UNKNOWN = '?';

  int rows = game_table.size();
  int cols = game_table[0].size();

  // Printing logic
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      MinesweeperCell current_cell = game_table[i][j];
      if (current_cell.revealed == true) {
        if (current_cell.mine == true) {
          std::cout << MINE;
        }
        else {
          std::cout << current_cell.neighbors;
        }
      }
      else {
        if (current_cell.flagged == 2) {
          std::cout << UNKNOWN;
        }
        else if (current_cell.flagged == 1) {
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

void make_move() {
  
}