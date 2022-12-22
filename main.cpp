#include "util_functions.cpp"

bool game_over = false;
bool game_saved = false;
int moves_left = 0;

int main() { start_game(); }
void start_game()
{
    auto [rows, cols, mines] = difficulty();
    game_over = false;
    moves_left = rows * cols - mines;
    std::vector<std::vector<MinesweeperCell>> game_field =
        create_game_field(rows, cols, mines);
    game_logic(game_field, mines);
}

void game_logic(std::vector<std::vector<MinesweeperCell>> game_table, int mines)
{
    /**
     * Handles the game logic
     *
     * @param game_table the game table itself.
     */

    const int rows = game_table.size();
    const int cols = game_table[0].size();

    while (game_over == false) {
        print_current_game_table(game_table, rows, cols);
        printf_s("Moves left: %i", moves_left);
        if (moves_left == 0) {
            game_over = true;
            std::cout << "You won!\n";
            break;
        }
        make_move(game_table, rows, cols, mines);
    }

    if (game_over == true) {
        ask_for_replay();
    }
}

void make_move(std::vector<std::vector<MinesweeperCell>>& game_table, int rows,
               int cols, int mines)
{
    char selection;
    std::cout << "\nMake a selection: \n";
    std::cout
        << "R - Reveal\nF - Flag\nN - New Game\nS - Save Game\nQ - Quit Game\n";
    std::cout << "> ";
    std::cin >> selection;

    if (selection == 'R' || selection == 'r') {
        reveal_cell(game_table, rows, cols, moves_left);
    }
    else if (selection == 'F' || selection == 'f') {
        flag_cell(game_table, rows, cols);
    }
    else if (selection == 'q' || selection == 'Q') {
        if (game_saved == false) {
            std::cout << "Warning, you have not saved your current game!\n";
        }
        else {
            std::cout << "You have saved your current game.\n";
        }
        char confirm_quit;
        std::cout << "Do you want to quit? (y/N) ";
        std::cin >> confirm_quit;
        if (confirm_quit == 'y' || confirm_quit == 'Y') {
            clear_screen();
            std::cout << "Thank you for playing!\n";
            exit(0);
        }
        else if (confirm_quit == 'n' || confirm_quit == 'N' ||
                 confirm_quit == ' ') {
            return;
        }
    }
    else if (selection == 's' || selection == 'S') {
        save_current_game(game_table, rows, cols, mines);
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
                 int rows, int cols, int& moves_left)
{
    auto [row, col] = ask_position();
    if (is_valid_cell(row, col, rows, cols) == false) {
        std::cout << "Your selection is out of bounds!\n";
    }
    else if (game_table[row][col].revealed == true ||
             game_table[row][col].flagged == FLAGGED ||
             game_table[row][col].flagged == UNSURE) {
        std::cout << "This cell cannot be opened!\n";
    }
    else {
        if (game_table[row][col].mine == true) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    game_table[i][j].revealed = true;
                }
            }
            print_current_game_table(game_table, rows, cols);
            std::cout << "Stepped on a mine! You lost!\n";
            game_over = true;
            sleep(3000);
        }
        else {
            game_table[row][col].revealed = true;
            moves_left--;
            if (game_table[row][col].neighbors == 0) {
                flood_fill(game_table, row, col, rows, cols, moves_left);
            }
        }
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

void ask_for_replay()
{
    char sel;
    std::cout << "Do you want to play again? (y/N) ";
    std::cin >> sel;

    if (sel == 'y' || sel == 'Y') {
        start_game();
    }
    else if (sel == 'n' || sel == 'N' || sel == ' ') {
        clear_screen();
        std::cout << "Thank you for playing!\n";
        exit(0);
    }
}

void save_current_game(std::vector<std::vector<MinesweeperCell>>& game_table,
                       int rows, int cols, int mines)
{
    std::cout << "Saving current game...\n";
    // open save file first:
    std::ofstream GameSaveFile;
    GameSaveFile.open("game_save.txt");     // I gave up trying to deal with
                                            // file paths in c++.

    if (GameSaveFile.is_open()) {
        // First line: row col mines
        GameSaveFile << rows << " " << cols << " " << mines << '\n';
        // Second lines: neighbors + mines
        // -1 for mines, 0 - 9 for mines
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int neighbors = game_table[i][j].neighbors;
                GameSaveFile << neighbors << " ";
            }
            GameSaveFile << "\n";
        }
        // Final lines: Revealed cells
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int current_cell = game_table[i][j].revealed;
                GameSaveFile << current_cell << " ";
            }
            GameSaveFile << "\n";
        }

        game_saved = true;
        std::cout << "Game has been saved.\n";
        sleep(1500);
    }
    else if (GameSaveFile.fail()) {
        std::cerr << "An error has occured while saving the game: \n";
        std::cerr << std::strerror(errno);
        sleep(1500);
    }

    GameSaveFile.close();
}
