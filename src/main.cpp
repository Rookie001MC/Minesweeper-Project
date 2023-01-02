/**
 * @file main.cpp
 * @author Nguyen Huu Quoc Thang - 22125091
 * @brief The main entry file for RookieSweeper
 * @date 2022-12-23
 *
 * RookieSweeper, because I have no idea what abomination I have coded in 10 weeks straight.
 *
 * @note This may be the worst code you may have ever seen, but, I haven't study C++ for a long
 * time, and I'm not even good at programming either.
 */
#include "minesweeper.hpp"

// Sets some global variables up just for easier access throughout the program
bool game_over              = false;
bool game_saved             = false;
int moves_left              = 0;
std::string current_exe_dir = "";
std::vector<std::vector<MinesweeperCell>> game_field;

int main(int argc, char *argv[])
{
    /**
     * @brief The main function.
     * Still does a lot of things before we can actually play the game.
     */
    print_ascii_banner();
    current_exe_dir = argv[0];  // Where is the game located?
    if (if_saved_file_exist(current_exe_dir))
    {
        std::cout << "There is a save file from last game. \nDo you want to "
                     "load it and continue playing? (y/n) ";
    load_save_file : {
        char sel = ' ';
        std::cin >> sel;
        if (sel == 'y' || sel == 'Y')
        {
            load_saved_game();
        }
        else if (sel == 'n' || sel == 'N')
        {
            // Removing the file in case we don't load the save game, to avoid collisions with next
            // launches
            std::filesystem::remove(get_save_file_path(current_exe_dir));
            start_new_game();
        }
        else
        {
            std::cout << "Selection is invalid!\nPlease enter Y or N: ";
            goto load_save_file;  // We can all agree this is ugly but I don't care, it works, I'm
                                  // bad at C++ ¯\_ (ツ)_/¯
        }
    }
    }
    else
    {
        start_new_game();
    }
}
void start_new_game()
{
    /**
     * @brief Starts a new game by asking for difficulty and create the game table
     */
    auto [rows, cols, mines] = difficulty();
    game_over                = false;
    moves_left               = rows * cols - mines;
    game_field               = create_new_game_field(rows, cols, mines);
    game_logic(game_field, mines);
}

void game_logic(std::vector<std::vector<MinesweeperCell>> game_table, int mines)
{
    /**
     * Handles the game logic
     *
     * @param game_table the game table itself.
     * @param mines Number of mines in the current game table
     */

    const int rows = game_table.size();
    const int cols = game_table[0].size();
    while (game_over == false)
    {
        print_current_game_table(game_table, rows, cols);
        printf("Moves left: %i", moves_left);
        if (moves_left == 0)
        {
            game_over = true;
            std::cout << "You won!\n";
            break;
        }
        make_move(game_table, rows, cols, mines);
    }

    if (game_over == true)
    {
        ask_for_replay();
    }
}

void make_move(std::vector<std::vector<MinesweeperCell>> &game_table, int rows, int cols, int mines)
{
    /**
     * @brief Asks the user for their next move
     *
     * @param rows Number of rows of the current game table
     * @param cols Number of cols of the current game table
     * @param mines Number of mines in the current game table
     */
    char selection;
    std::cout << "\nMake a selection: \n";
    std::cout << "R - Reveal\n"
              << "F - Flag\n"
              << "S - Save Game\n"
              << "Q - Quit Game\n";
    std::cout << "> ";
    std::cin >> selection;

    if (selection == 'R' || selection == 'r')
    {
        reveal_cell(game_table, rows, cols, moves_left);
    }
    else if (selection == 'F' || selection == 'f')
    {
        flag_cell(game_table, rows, cols);
    }
    else if (selection == 'q' || selection == 'Q')
    {
        if (game_saved == false)
        {
            std::cout << "Warning, you have not saved your current game!\n";
        }
        else
        {
            std::cout << "You have saved your current game.\n";
        }
        char confirm_quit;
        std::cout << "Do you want to quit? (y/N) ";
        std::cin >> confirm_quit;
        if (confirm_quit == 'y' || confirm_quit == 'Y')
        {
            quit_game();
        }
        else if (confirm_quit == 'n' || confirm_quit == 'N' || confirm_quit == ' ')
        {
            return;
        }
    }
    else if (selection == 's' || selection == 'S')
    {
        save_current_game(game_table, rows, cols, mines, moves_left);
    }
    else
    {
        std::cout << "Selection is invalid!";
    }
    sleep(500);
    clear_screen();
}

void reveal_cell(std::vector<std::vector<MinesweeperCell>> &game_table,
                 int rows,
                 int cols,
                 int &moves_left)
{
    /**
     * @brief Ask for a position in the game board, then do some checks to reveal the specified cell
     *
     * @param game_table Current game table
     * @param rows Number of rows of the current game table
     * @param cols Number of columns of the current game table
     * @param moves_left Numbers of moves left in this game table
     */
    auto [row, col] = ask_position();
    if (is_valid_cell(row, col, rows, cols) == false)  // Cell is flagged
    {
        std::cout << "Your selection is out of bounds!\n";
    }
    else if (game_table[row][col].revealed == true || game_table[row][col].flagged == FLAGGED ||
             game_table[row][col].flagged ==
                 UNSURE)  // Cell is already flagged/revealed/marked as unsure
    {
        std::cout << "This cell cannot be opened!\n";
    }
    else  // Normal state
    {
        if (game_table[row][col].mine == true)  // *boom*
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    // I may actually consider only revealing the mines only but my current setup is
                    // not worth doing that
                    game_table[i][j].revealed = true;
                }
            }
            print_current_game_table(game_table, rows, cols);
            std::cout << "Stepped on a mine! You lost!\n";
            game_over = true;
            sleep(3000);  // Goes back to `game_logic()`
        }
        else  // Safe
        {
            game_table[row][col].revealed = true;
            moves_left--;
            if (game_table[row][col].neighbors == 0)
            {
                // How the f did I wasted so much time on this
                flood_fill(game_table, row, col, rows, cols, moves_left);
            }
        }
    }
    return;
}

void flag_cell(std::vector<std::vector<MinesweeperCell>> &game_table, int rows, int cols)
{
    /**
     * @brief Same as `reveal_cell()`, but flags cells instead of revealing them
     *
     * @param game_table Current game table
     * @param rows Number of rows of the current game table
     * @param cols Number of columns of the current game table
     */
    auto [row, col] = ask_position();
    if (is_valid_cell(row, col, rows, cols) == false)
    {
        std::cout << "Your selection is out of bounds!\n";
    }
    else if (game_table[row][col].revealed == true)
    {
        std::cout << "This cell cannot be flagged!\n";
    }
    else
    {
        if (game_table[row][col].flagged == flag_status::UNFLAGGED)
        {
            game_table[row][col].flagged = flag_status::FLAGGED;
        }
        else if (game_table[row][col].flagged == flag_status::FLAGGED)
        {
            game_table[row][col].flagged = flag_status::UNSURE;
        }
        else if (game_table[row][col].flagged == flag_status::UNSURE)
        {
            game_table[row][col].flagged = flag_status::UNFLAGGED;
        }
    }
    return;
}

void ask_for_replay()
{
    /**
     * @brief Ask the user if they want to play a new game
     */
    char sel;
    std::cout << "Do you want to play again? (y/N) ";
    std::cin >> sel;

    if (sel == 'y' || sel == 'Y')
    {
        start_new_game();
    }
    else if (sel == 'n' || sel == 'N')
    {
        quit_game();
    }
}

void save_current_game(std::vector<std::vector<MinesweeperCell>> &game_table,
                       int rows,
                       int cols,
                       int mines,
                       int moves_left)
{
    /**
     * @brief Save the current game when the user choose the Save feature
     *
     * @param rows Number of rows of the current game table
     * @param cols Number of cols of the current game table
     * @param mines Number of mines in the current game table
     * @param moves_left Number of moves left in this current game table
     *
     */

    std::cout << "Saving current game...\n";
    // Getting the current EXE path
    std::string save_file_loc = get_save_file_path(current_exe_dir);

    // open save file first:
    std::ofstream GameSaveFile;
    GameSaveFile.open(save_file_loc);

    if (GameSaveFile.is_open())
    {
        // First line: row col mines
        GameSaveFile << rows << " " << cols << " " << mines << '\n';
        // Second line: moves left
        GameSaveFile << moves_left << "\n";
        // Third lines: neighbors + mines
        // -1 for mines, 0 - 9 for mines
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int neighbors = game_table[i][j].neighbors;
                GameSaveFile << neighbors << " ";
            }
            GameSaveFile << "\n";
        }
        // Final lines: Revealed/flagged cells
        // -2 for unsures, -1 for flagged, 0 for unrevealed, 1 for revealed
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                MinesweeperCell current_cell = game_table[i][j];
                if (current_cell.flagged == flag_status::UNSURE)
                {
                    GameSaveFile << -2 << " ";
                }
                else if (current_cell.flagged == flag_status::FLAGGED)
                {
                    GameSaveFile << -1 << " ";
                }
                else
                {
                    GameSaveFile << current_cell.revealed << " ";
                }
            }
            GameSaveFile << "\n";
        }

        game_saved = true;
        std::cout << "Game has been saved.\n";
        sleep(1500);
    }
    else if (GameSaveFile.fail())
    {
        std::cerr << "An error has occured while saving the game: \n";
        std::cerr << std::strerror(errno);
        sleep(1500);
    }

    GameSaveFile.close();
}

void load_saved_game()
{
    /**
     * @brief Load a saved game, if it is found in the same directory as where the game is located.
     */
    std::string save_file_loc = get_save_file_path(current_exe_dir);

    std::ifstream GameLoadFile;
    GameLoadFile.open(save_file_loc);

    int rows, cols, mines;
    if (GameLoadFile.is_open())
    {
        printf("Loaded save file correctly!\n");
        // Load the rows, cols and mine positions
        GameLoadFile >> rows >> cols >> mines >> moves_left;

        // Initialize an empty game table as soon as the file has successfuly
        // opened
        game_field =
            std::vector<std::vector<MinesweeperCell>>(rows, std::vector<MinesweeperCell>(cols));

        // Load the mine/neighbors values
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int current_cell_data;
                GameLoadFile >> current_cell_data;
                if (current_cell_data == -1)
                {
                    game_field[i][j].mine = true;
                }
                game_field[i][j].neighbors = current_cell_data;
            }
        }

        // Load the revealed/flagged status
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int current_cell_data;
                GameLoadFile >> current_cell_data;
                if (current_cell_data == -2)
                {
                    game_field[i][j].flagged = flag_status::UNSURE;
                }
                else if (current_cell_data == -1)
                {
                    game_field[i][j].flagged = flag_status::FLAGGED;
                }
                else
                {
                    game_field[i][j].revealed = current_cell_data;
                }
            }
        }
        GameLoadFile.close();
        std::cout << "Game loaded successfully! Continuing game...";
        sleep(2000);
        game_logic(game_field, mines);
    }
    else if (GameLoadFile.fail())
    {
        std::cerr << "An error has occured while loading the game\n";
        std::cerr << std::strerror(errno);

        std::cout << "Starting a new game instead...";
        sleep(1500);
        start_new_game();
    }
}

void print_ascii_banner()
{
    /**
     * @brief Prints out a very nice ASCII art banner.
     */
    std::cout << R"(
8888888b.                 888     d8b         .d8888b.                                                      
888   Y88b                888     Y8P        d88P  Y88b                                                     
888    888                888                Y88b.                                                          
888   d88P .d88b.  .d88b. 888  888888 .d88b.  "Y888b.  888  888  888 .d88b.  .d88b. 88888b.  .d88b. 888d888 
8888888P" d88""88bd88""88b888 .88P888d8P  Y8b    "Y88b.888  888  888d8P  Y8bd8P  Y8b888 "88bd8P  Y8b888P"   
888 T88b  888  888888  888888888K 88888888888      "888888  888  8888888888888888888888  88888888888888     
888  T88b Y88..88PY88..88P888 "88b888Y8b.    Y88b  d88PY88b 888 d88PY8b.    Y8b.    888 d88PY8b.    888     
888   T88b "Y88P"  "Y88P" 888  888888 "Y8888  "Y8888P"  "Y8888888P"  "Y8888  "Y8888 88888P"  "Y8888 888     
                                                                                    888                     
                                                                                    888                     
                                                                                    888

)";
    std::cout << "Made by Nguyen Huu Quoc Thang (Rookie Nguyen)\n";
}