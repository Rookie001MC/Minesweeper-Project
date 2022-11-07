#include <iostream>
#include <random>
#include <chrono>
#include <list>

int create_field(int rows, int cols, int mines);
int random_num_gen(int from, int to);
int main(void)
{
  // Take in the user input of X, Y and mine numbers
  int x, y;
  int mines;
  std::cout << "Enter number of rows (1-100): ";
  std::cin >> x;
  std::cout << "Enter number of columns (1-100): ";
  std::cin >> y;

  std::cout << "Enter numbe of mines (1-99): ";
  std::cin >> mines;

  create_field(x, y, mines);
}

int create_field(int rows, int cols, int mines)
{
  int board[rows][cols] = {};

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      board[i][j] = 0;
    }
  }

  struct Mine_pos
  {
    int row;
    int col;
  }; // Because we can only have so many mines as the user's input.

  Mine_pos mine_positions[mines];
  int current_mines = 0;
  while (current_mines < mines)
  {
    int row = random_num_gen(0, rows);
    int col = random_num_gen(0, cols);

    Mine_pos current_rand_pos;
    current_rand_pos.row = row;
    current_rand_pos.col = col;
  }
}

int random_num_gen(int from, int to)
{
  // Reference: https://learn.microsoft.com/en-us/cpp/standard-library/random?view=msvc-170
  std::random_device rd;
  std::mt19937 rng_engine(rd()); // Yes this is overkill af
  std::uniform_int_distribution<int> dist(from, to);

  return dist(rng_engine);
}