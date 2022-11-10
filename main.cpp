#include <iostream>
#include <random>
#include <chrono>
#include "minesweeper.hpp"
#include <vector>

int internal_minefield[100][100];
int empty_field[100][100];
int final_field[100][100];

int create_field(int rows, int cols, int mines);
int random_num_gen(int from, int to);
int get_mine_neighbors(int row, int col, int rows, int cols);
int main(void)
{
	// Take in the user input of X, Y and mine numbers
	int x, y;
	int mines;
	std::cout << "Enter number of rows (1-100): ";
	std::cin >> x;
	std::cout << "Enter number of cols (1-100): ";
	std::cin >> y;

	std::cout << "Enter numbe of mines (1-99): ";
	std::cin >> mines;

	create_field(x, y, mines);
}

int create_field(int rows, int cols, int mines)
{
	/**
	 * Creates the `board`, which contains the cell data and mine positions,
	 * as well as the `player_field`, which, by default, all the mine data is completely
	 * hidden from the player.
	 *
	 * @param rows Number of rows
	 * @param cols Number of mines
	 * @param mines Number of mines
	 *
	 * @return still have no idea yet
	 */

	// Creates the solution field
	std::vector<std::vector<MinesweeperCell>> table(rows, std::vector<MinesweeperCell>(cols));

	// Creates the player field
	std::vector<std::vector<MinesweeperCell>> player_field(rows, std::vector<MinesweeperCell>(cols));

	std::cout << "\nCreating a hypothetical Minesweeper field with " << rows << "x" << cols << ":\n";
	// Fill in the board with MinesweeperCells
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			table[i][j] = MinesweeperCell();
		}
	}

	// Generate the mine positions:
	for (int current_mine = 0; current_mine < mines; current_mine++)
	{
		int random_bomb_row = random_num_gen(0, rows);
		int random_bomb_col = random_num_gen(0, cols);

		std::cout << "Random mine pos: " << random_bomb_row << " - " << random_bomb_col << "\n";

		if (table[random_bomb_row][random_bomb_col].mine == true)
		{
			continue;
		}
		else
		{
			table[random_bomb_row][random_bomb_col].mine = true;

			if (random_bomb_row != 0 && random_bomb_col != 0) // check for edge
				table[random_bomb_row - 1][random_bomb_col - 1].neighbors += 1;

			// for area "2"
			if (random_bomb_row != 0) // check for edge
				table[random_bomb_row - 1][random_bomb_col].neighbors += 1;

			// for area "3"
			if (random_bomb_row != 0 && random_bomb_col != cols - 1) // check for edge
				table[random_bomb_row - 1][random_bomb_col + 1].neighbors += 1;

			// for area "4"
			if (random_bomb_col != 0) // check for edge
				table[random_bomb_row][random_bomb_col - 1].neighbors += 1;

			// for area "5"
			if (random_bomb_col != cols - 1) // check for edge
				table[random_bomb_row][random_bomb_col + 1].neighbors += 1;

			// for area "6"
			if (random_bomb_row != rows - 1 && random_bomb_col != 0) // check for edge
				table[random_bomb_row + 1][random_bomb_col - 1].neighbors += 1;

			// for area "7"
			if (random_bomb_row != rows - 1) // check for edge
				table[random_bomb_row + 1][random_bomb_col].neighbors += 1;

			// for area "8"
			if (random_bomb_row != rows - 1 && random_bomb_col != cols - 1) // check for edge
				table[random_bomb_row + 1][random_bomb_col + 1].neighbors += 1;
		}
	}

	// Debug code for visualising game cells
	std::cout << "Solution table: \n";
	std::cout << "Note: format of each cell is: Cell(mine_status, revealed_status, neighboring_mines)\n\n";
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << "Cell(" << table[i][j].mine << ", " << table[i][j].revealed << ", " << table[i][j].neighbors << ")\t";
		}
		std::cout << "\n";
	}
	return 0;
}
int random_num_gen(int from, int to)
{
	/**
	 * A *really really* overkill number generator function, using the Mersenne Twister engine.
	 * Generates an integer ranging `from` to `to`, exclusively.
	 * Reference: https://learn.microsoft.com/en-us/cpp/standard-library/random?view=msvc-170
	 *
	 * @param from The starting number of the range
	 * @param to The ending number of the range
	 *
	 * @return a new random integer
	 */

	std::random_device rd;
	std::mt19937 rng_engine(rd());
	std::uniform_int_distribution<int> dist(from, to - 1);

	return dist(rng_engine);
}