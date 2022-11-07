typedef struct Minesweeper {
    unsigned short int rows;
    unsigned short int cols;
    unsigned short int mines;

    short int **board;
    char **mask;

} Minesweeper;
