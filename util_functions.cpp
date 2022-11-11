#include <Windows.h>
#include <iostream>
#include <random>
#include <tuple>
int random_num_gen(int from, int to)
{
    /**
     * A *really really* overkill number generator function, using the Mersenne
     * Twister engine. Generates an integer ranging `from` to `to`, exclusively.
     * Reference:
     * https://learn.microsoft.com/en-us/cpp/standard-library/random?view=msvc-170
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
void print_help_sel()
{
    std::cout << "Pick a grid preset: \n";
    std::cout << "0. Custom (default)\n";
    std::cout << "1. Easy - 8x8 - 10 mines\n";
    std::cout << "2. Intermediate - 16x16 - 40 mines\n";
    std::cout << "3. Hard - 30x16 - 99 mines\n";
}
std::tuple<int, int, int> difficulty()
{
    unsigned int height, width;
    unsigned int mines;
    unsigned int preset = 0;

    print_help_sel();
    std::cin >> preset;

    while (preset < 0 || preset > 3) {
        std::cout << "That option is unrecognized!\n";
        Sleep(2000);
        print_help_sel();
        std::cin >> preset;
    }

    if (preset == 0) {
        std::cout << "Enter the height of the grid: ";
        std::cin >> height;
        std::cout << "Enter the width of the grid: ";
        std::cin >> width;
        std::cout << "Enter the number of mines to randomize: ";
        std::cin >> mines;
    } else if (preset == 1) {
        height = 8;
        width = 8;
        mines = 10;
    } else if (preset == 2) {
        height = 16;
        width = 16;
        mines = 40;
    } else if (preset == 3) {
        height = 40;
        width = 16;
        mines = 99;
    }

    return std::make_tuple(height, width, mines);
}