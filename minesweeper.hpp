// =================== HEADERS =================== //
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <tuple>
#include <vector>

// =============== DATA STRUCTURES =============== //
enum flag_status {
    /**
     * Defines the status for the current Cell.
     */
    UNFLAGGED = 0,
    FLAGGED = 1,
    UNSURE = 2
};
typedef struct MinesweeperCell {
    /**
     * Defines a Cell in the Minesweeper game.
     */
    bool mine = false;         // If it's a mine.
    bool revealed = false;     // If it has been revealed.
    flag_status flagged =
        UNFLAGGED;               // If it has been flagged, defined above.
    short int neighbors = 0;     // How many mines are neighboring the square.
} MinesweeperCell;

// ============= FUNCTION DECLARATIONS ============= //
void game_logic(std::vector<std::vector<MinesweeperCell>> game_table,
                int mines);
int random_num_gen(int from, int to);
void print_help_sel();
std::tuple<int, int, int> difficulty();
void start_game();
void print_current_game_table(
    std::vector<std::vector<MinesweeperCell>>& game_table, int rows, int cols);
void make_move(std::vector<std::vector<MinesweeperCell>>& game_table, int rows,
               int cols, int mines);
void reveal_cell(std::vector<std::vector<MinesweeperCell>>& game_table,
                 int rows, int cols, int& moves_left);
void flag_cell(std::vector<std::vector<MinesweeperCell>>& game_table, int rows,
               int cols);
std::vector<std::vector<MinesweeperCell>> create_game_field(int rows, int cols,
                                                            int mines);
bool is_valid_cell(int row, int col, int rows, int cols, int mines);
void save_current_game(std::vector<std::vector<MinesweeperCell>>& game_table,
                       int rows, int cols, int mines);
void ask_for_replay();
void clear_screen();
void sleep(int milliseconds);
