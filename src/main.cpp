#include "minesweeper.hpp"

bool game_over              = false;
bool game_saved             = false;
int moves_left              = 0;
std::string current_exe_dir = "";
std::vector<std::vector<MinesweeperCell>> game_field;
int main(int argc, char *argv[])
{
    current_exe_dir = argv[0];
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
            std::filesystem::remove(get_save_file_path(current_exe_dir));
            start_new_game();
        }
        else
        {
            std::cout << "Selection is invalid!\nPlease enter Y or N: ";
            goto load_save_file;
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
    char selection;
    std::cout << "\nMake a selection: \n";
    std::cout << "R - Reveal\nF - Flag\nN - New Game\nS - Save Game\nQ - Quit Game\n";
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
            clear_screen();
            std::cout << "Thank you for playing!\n";
            exit(0);
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
        std::cout << "This feature is either not implemented yet, or the selection "
                     "is invalid!";
    }
    sleep(500);
    clear_screen();
}

void reveal_cell(std::vector<std::vector<MinesweeperCell>> &game_table,
                 int rows,
                 int cols,
                 int &moves_left)
{
    auto [row, col] = ask_position();
    if (is_valid_cell(row, col, rows, cols) == false)
    {
        std::cout << "Your selection is out of bounds!\n";
    }
    else if (game_table[row][col].revealed == true || game_table[row][col].flagged == FLAGGED ||
             game_table[row][col].flagged == UNSURE)
    {
        std::cout << "This cell cannot be opened!\n";
    }
    else
    {
        if (game_table[row][col].mine == true)
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    game_table[i][j].revealed = true;
                }
            }
            print_current_game_table(game_table, rows, cols);
            std::cout << "Stepped on a mine! You lost!\n";
            game_over = true;
            sleep(3000);
        }
        else
        {
            game_table[row][col].revealed = true;
            moves_left--;
            if (game_table[row][col].neighbors == 0)
            {
                flood_fill(game_table, row, col, rows, cols, moves_left);
            }
        }
    }
    return;
}

void flag_cell(std::vector<std::vector<MinesweeperCell>> &game_table, int rows, int cols)
{
    auto [row, col] = ask_position();
    if (row > rows || row <= 0 || col > cols || col <= 0)
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
    char sel;
    std::cout << "Do you want to play again? (y/N) ";
    std::cin >> sel;

    if (sel == 'y' || sel == 'Y')
    {
        start_new_game();
    }
    else if (sel == 'n' || sel == 'N' || sel == ' ')
    {
        clear_screen();
        std::cout << "Thank you for playing!\n";
        exit(0);
    }
}

void save_current_game(std::vector<std::vector<MinesweeperCell>> &game_table,
                       int rows,
                       int cols,
                       int mines,
                       int moves_left)
{
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
        start_new_game();
        sleep(1500);
    }
}
