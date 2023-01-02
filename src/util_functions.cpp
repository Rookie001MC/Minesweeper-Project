/**
 * @file util_functions.cpp
 * @author Nguyen Huu Quoc Thang
 * @brief Contains the functions that are necessary for the functionality of the game, but shouldn't
 * be put into the main file
 * @date 2022-12-23
 *
 * RookieSweeper, because I have no idea what abomination I have coded in 10 weeks straight.
 *
 * @note This may be the worst code you may have ever seen, but, I haven't study C++ for a long
 * time, and I'm not even good at programming either.
 */
#include "minesweeper.hpp"

std::vector<std::vector<MinesweeperCell>> create_new_game_field(int rows, int cols, int mines)
{
    /**
     * @brief Creates the `board`, which contains the cell data and mine
     * positions,
     *
     * @param rows Number of rows
     * @param cols Number of mines
     * @param mines Number of mines
     *
     * @return The game table with pre-filled data (mine locations, neighbors, ...)
     */

    // Creates the solution field
    std::vector<std::vector<MinesweeperCell>> table(rows, std::vector<MinesweeperCell>(cols));

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

        // Check if the selected position has a mine already
        if (table[random_bomb_row][random_bomb_col].mine == true)
        {
            continue;
        }
        else
        {  // Set the randomized position to be a mine
            table[random_bomb_row][random_bomb_col].mine      = true;
            table[random_bomb_row][random_bomb_col].neighbors = -1;

            // Find all the neighboring cells of the mine, then add up
            // the neighbor counts for each neighboring cells
            if (random_bomb_row != 0 && random_bomb_col != 0)
            {  // North - West
                table[random_bomb_row - 1][random_bomb_col - 1].neighbors += 1;
            }

            if (random_bomb_row != 0)
            {  // North
                table[random_bomb_row - 1][random_bomb_col].neighbors += 1;
            }

            if (random_bomb_row != 0 && random_bomb_col != cols - 1)
            {  // North - East
                table[random_bomb_row - 1][random_bomb_col + 1].neighbors += 1;
            }

            if (random_bomb_col != cols - 1)
            {  // East
                table[random_bomb_row][random_bomb_col + 1].neighbors += 1;
            }

            if (random_bomb_row != rows - 1 && random_bomb_col != cols - 1)
            {  // South - East
                table[random_bomb_row + 1][random_bomb_col + 1].neighbors += 1;
            }

            if (random_bomb_row != rows - 1)
            {  // South
                table[random_bomb_row + 1][random_bomb_col].neighbors += 1;
            }

            if (random_bomb_row != rows - 1 && random_bomb_col != 0)
            {  // South - West
                table[random_bomb_row + 1][random_bomb_col - 1].neighbors += 1;
            }

            if (random_bomb_col != 0)
            {  // West
                table[random_bomb_row][random_bomb_col - 1].neighbors += 1;
            }
        }
    }

    return table;
}

void print_current_game_table(std::vector<std::vector<MinesweeperCell>> &game_table,
                              int rows,
                              int cols)
{
    /**
     * @brief Print to stdout the current game table.
     *
     * @param game_table Current game table
     * @param rows Number of rows
     * @param cols Number of columns
     *
     */

    // Characters to be used for printing to console
    const char UNREVEALED = '.';
    const char MINE       = '*';
    const char FLAGGED    = 'F';
    const char UNKNOWN    = '?';

    clear_screen();

    /**
     * @brief The printing logic for the game table
     *
     * @note Since we need to
     */
    for (int i = 0; i <= rows; i++)
    {
        if (i == 0)
        {
            for (int col_num = 0; col_num <= rows; col_num++)
            {
                col_num == 0 ? std::cout << " "
                                         << "\t"
                             : std::cout << col_num << "\t";
            }
            std::cout << "\n";
        }
        else
        {
            for (int j = 0; j <= cols; j++)
            {
                if (j == 0)
                {
                    std::cout << i << "\t";
                }
                else
                {
                    if (game_table[i - 1][j - 1].revealed == true)
                    {
                        if (game_table[i - 1][j - 1].mine == true)
                        {
                            std::cout << MINE;
                        }
                        else
                        {
                            game_table[i - 1][j - 1].neighbors == 0
                                ? std::cout << " "
                                : std::cout << game_table[i - 1][j - 1].neighbors;
                        }
                    }
                    else
                    {
                        if (game_table[i - 1][j - 1].flagged == flag_status::UNSURE)
                        {
                            std::cout << UNKNOWN;
                        }
                        else if (game_table[i - 1][j - 1].flagged == flag_status::FLAGGED)
                        {
                            std::cout << FLAGGED;
                        }
                        else
                        {
                            std::cout << UNREVEALED;
                        }
                    }
                    std::cout << "\t";
                }
            }
            std::cout << "\n";
        }
    }
}

void flood_fill(std::vector<std::vector<MinesweeperCell>> &game_table,
                int row,
                int col,
                int rows,
                int cols,
                int &moves_left)
{
    /**
     * @brief Flood fill algorithm, also known as how did i wasted so much time just for this one
     * thing
     *
     * @param game_table Current game table
     * @param row User-inputted row position
     * @param col User-inputted col position
     * @param rows Number of rows of the current game table
     * @param cols Number of cols of the current game table
     */
    for (int col_off = -1; col_off <= 1; col_off++) // y-axis
    {
            for (int row_off = -1; row_off <= 1; row_off++) // x-axis
        {
            int next_col = col + col_off;
            int next_row = row + row_off;
            if (next_col > -1 && next_col < cols && next_row > -1 && next_row < rows)
            {
                if (game_table[next_row][next_col].mine == false &&
                    game_table[next_row][next_col].revealed == false &&
                    game_table[row][col].neighbors == 0)
                {
                    game_table[next_row][next_col].revealed = true;
                    moves_left--;
                    flood_fill(game_table, next_row, next_col, rows, cols, moves_left);
                }
            }
        }
    }
}

int random_num_gen(int from, int to)
{
    /**
     * @brief Generates a random number in a specified range.
     * A *really really* overkill number generator function, using the
     * Mersenne Twister engine. Generates an integer ranging `from` to `to`,
     * exclusively.
     * @see https://learn.microsoft.com/en-us/cpp/standard-library/random?view=msvc-170
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

std::tuple<int, int, int> difficulty()
{
    /**
     * @brief Ask the user for the desired difficulty to be played
     * @note These difficuty settings are based on the original Microsoft Minesweeper game.
     * @return A std::tuple containing 3 values: height, width, and number of mines
     */
    unsigned int height, width;
    unsigned int mines;
    char preset = '0';

    print_help_sel();
    std::cin >> preset;

    while (preset != '0' && preset != '1' && preset != '2' && preset != '3' && preset != 'q' &&
           preset != 'Q')  // I suck, yes
    {
        std::cout << "That option is unrecognized!\n";
        sleep(2000);
        print_help_sel();
        preset = '0';
        std::cin >> preset;
    }

    if (preset == '0')
    {
        std::cout << "Enter the height of the grid: ";
        std::cin >> height;
        std::cout << "Enter the width of the grid: ";
        std::cin >> width;
        std::cout << "Enter the number of mines to randomize: ";
        std::cin >> mines;
    }
    else if (preset == '1')
    {
        height = 8;
        width  = 8;
        mines  = 10;
    }
    else if (preset == '2')
    {
        height = 16;
        width  = 16;
        mines  = 40;
    }
    else if (preset == '3')
    {
        height = 40;
        width  = 16;
        mines  = 99;
    }
    else if (preset == 'q' || preset == 'Q')
    {
        quit_game();
    }

    return std::make_tuple(height, width, mines);
}

std::tuple<int, int> ask_position()
{
    /**
     * @brief Ask the user for position for further processing
     *
     * @return Tuple consisting of the user input - 1
     */

    unsigned int row = 0, col = 0;
    std::cout << "Enter your desired position (row, column): ";
    std::cin >> row >> col;
    return std::make_tuple(row - 1, col - 1);
}

bool is_valid_cell(int row, int col, int rows, int cols)
{
    /**
     * @brief Check if the cell the the user specified is valid.
     *
     * @param row User-inputted row position
     * @param col User-inputted col position
     * @param rows Number of rows of the current game table
     * @param cols Number of cols of the current game table
     *
     * @return True if valid, false if not.
     */
    if (row >= rows || row < 0 || col >= cols || col < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool if_saved_file_exist(std::string current_dir)
{
    /**
     * @brief Check the current directory where the game is currently located
     * @param current_dir The current directory where the game is located
     * @return True if the save file exists, false if not.
     */
    std::string save_file_loc = get_save_file_path(current_dir);
    return (std::filesystem::exists(save_file_loc));
}

std::string get_save_file_path(std::string current_dir)
{
    /**
     * @brief Append the current game location with the hard-coded save file name.
     * @param current_dir The current directory where the game is located
     * @return std::string of current game directory
     */
    return (get_current_game_location(current_dir) / "game_save.txt").generic_string();
}

std::filesystem::path get_current_game_location(std::string current_dir)
{
    /**
     * @brief Gets the directory where the game is currently located
     * @param current_dir The current directory where the game is located
     * @return std::string of current game directory
     */
    return (std::filesystem::path(current_dir).parent_path());
}

void print_help_sel()
{
    std::cout << "Pick a grid preset: \n";
    std::cout << "0. Custom\n";
    std::cout << "1. Easy - 8x8 - 10 mines\n";
    std::cout << "2. Intermediate - 16x16 - 40 mines\n";
    std::cout << "3. Hard - 30x16 - 99 mines\n";
    std::cout << "Q - Quit Game\n";
    std::cout << "> ";
}

void clear_screen()
{
    /**
     * @brief Clear the screen, the crossplatform-er and safer way.
     *
     * @see https://stackoverflow.com/a/32008479
     * @bug This will only work with systems with ANSI support (Win10 Build 1511
     * or later, Linux, MacOS).
     *
     */

    std::cout << "\033[2J\033[1;1H";
}

void sleep(int milliseconds)
{
    /** @brief Pause execution of the program, similar to Windows.h's Sleep(),
     * the crossplatform-er way.
     *
     * @see https://stackoverflow.com/a/11276503
     * @param milliseconds Number of milliseconds to pause execution
     *
     */

    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void quit_game()
{
    /**
     * @brief Exits the game.
     */
    {
        clear_screen();
        std::cout << "Thank you for playing!\n";
        sleep(1500);
        exit(0);
    }
}