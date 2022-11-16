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
    bool mine = false;                // If it's a mine.
    bool revealed = false;            // If it has been revealed.
    flag_status flagged = UNFLAGGED;  // If it has been flagged, defined above.
    unsigned short int neighbors = 0; // How many mines are neighboring the square.
} MinesweeperCell;
