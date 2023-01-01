/**
 * @file util_functions.cpp
 * @author Nguyen Huu Quoc Thang
 * @brief
 * @date 2022-12-23
 *
 * RookieSweeper, because I have no idea what abomination I have coded in 10
 * weeks straight.
 *
 * @note This may be the worst code you may have ever seen, but, I haven't study
 * C++ for a long time, and I'm not even good at programming either.
 */
// =================== HEADERS =================== //
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>  // For `sleep()`
#include <tuple>
#include <vector>

// =============== DATA STRUCTURES =============== //
enum flag_status
{
    /**
     * @brief Defines the status for the current Cell.
     */
    UNFLAGGED = 0,
    FLAGGED   = 1,
    UNSURE    = 2
};
typedef struct MinesweeperCell
{
    /**
     * @brief Defines a Cell in the Minesweeper game.
     */
    bool mine           = false;      // If it's a mine.
    bool revealed       = false;      // If it has been revealed.
    flag_status flagged = UNFLAGGED;  // If it has been flagged, defined above.
    short int neighbors = 0;          // How many mines are neighboring the square.
} MinesweeperCell;

// ============= GAMEPLAY FUNCTIONS ============= //
void game_logic(std::vector<std::vector<MinesweeperCell>> game_table, int mines);
void start_new_game();
void make_move(std::vector<std::vector<MinesweeperCell>> &game_table,
               int rows,
               int cols,
               int mines);
void reveal_cell(std::vector<std::vector<MinesweeperCell>> &game_table,
                 int rows,
                 int cols,
                 int &moves_left);
void flag_cell(std::vector<std::vector<MinesweeperCell>> &game_table, int rows, int cols);
void flood_fill(std::vector<std::vector<MinesweeperCell>> &game_table,
                int row,
                int col,
                int rows,
                int cols,
                int &moves_left);
void save_current_game(std::vector<std::vector<MinesweeperCell>> &game_table,
                       int rows,
                       int cols,
                       int mines,
                       int moves_left);

// ======== GAME-RELATED FUNCTIONS ========= //
std::tuple<int, int, int> difficulty();
std::tuple<int, int> ask_position();
void print_current_game_table(std::vector<std::vector<MinesweeperCell>> &game_table,
                              int rows,
                              int cols);
bool is_valid_cell(int row, int col, int rows, int cols);
std::vector<std::vector<MinesweeperCell>> create_new_game_field(int rows, int cols, int mines);

// ======== UTILITY FUNCTIONS ========== //
void print_ascii_banner();
void print_help_sel();
int random_num_gen(int from, int to);
void load_saved_game();
void ask_for_replay();
void clear_screen();
void sleep(int milliseconds);
bool if_saved_file_exist(std::string current_dir);
std::filesystem::path get_current_game_location(std::string current_dir);
std::string get_save_file_path(std::string current_dir);
void quit_game();
