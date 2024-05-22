#ifndef BOARD_H
#define BOARD_H

#define NUM_CELL 10

typedef enum {
    PAWN_NULL = -1,
    PAWN_WHITE = 0,
    PAWN_BLACK = 1,
    QUEEN_WHITE = 2,
    QUEEN_BLACK = 3
} PawnType;

typedef struct {
    int row;
    int col;
    int toRow;
    int toCol;
    int capturedRow;
    int capturedCol;
    PawnType type;
} Move;

void initBoard(PawnType board[NUM_CELL][NUM_CELL]);
void printBoard(PawnType board[NUM_CELL][NUM_CELL]);
int isValidMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol);
void makeMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol);
void convertCoordinate(char* coord, int* row, int* col);
int canCapture(PawnType board[NUM_CELL][NUM_CELL], int row, int col);
int getCaptureMoves(PawnType board[NUM_CELL][NUM_CELL], int row, int col, Move captureMoves[]);
int hasNoPawns(PawnType board[NUM_CELL][NUM_CELL], int player);
int checkWinner(PawnType board[NUM_CELL][NUM_CELL]);

#endif // BOARD_H
