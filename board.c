#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initBoard(PawnType board[NUM_CELL][NUM_CELL]) {
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (i < 4 && (i + j) % 2 == 1) {
                board[i][j] = PAWN_BLACK;
            } else if (i > 5 && (i + j) % 2 == 1) {
                board[i][j] = PAWN_WHITE;
            } else {
                board[i][j] = PAWN_NULL;
            }
        }
    }
}

void printBoard(PawnType board[NUM_CELL][NUM_CELL]) {
    printf("    a   b   c   d   e   f   g   h   i   j\n");
    for (int i = 0; i < NUM_CELL; i++) {
        printf("  +---+---+---+---+---+---+---+---+---+---+\n");
        printf("%2d ", i + 1);
        for (int j = 0; j < NUM_CELL; j++) {
            printf("|");
            switch(board[i][j]) {
                case PAWN_NULL:
                    printf("   ");
                    break;
                case PAWN_WHITE:
                    printf(" w ");
                    break;
                case PAWN_BLACK:
                    printf(" b ");
                    break;
                case QUEEN_WHITE:
                    printf(" W ");
                    break;
                case QUEEN_BLACK:
                    printf(" B ");
                    break;
            }
        }
        printf("|\n");
    }
    printf("  +---+---+---+---+---+---+---+---+---+---+\n");
}

int isValidMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol) {
    if (toRow < 0 || toRow >= NUM_CELL || toCol < 0 || toCol >= NUM_CELL) {
        return 0;
    }
    if (board[toRow][toCol] != PAWN_NULL) {
        return 0;
    }
    if (board[fromRow][fromCol] == PAWN_WHITE && toRow == fromRow - 1 && (toCol == fromCol - 1 || toCol == fromCol + 1)) {
        return 1;
    }
    if (board[fromRow][fromCol] == PAWN_BLACK && toRow == fromRow + 1 && (toCol == fromCol - 1 || toCol == fromCol + 1)) {
        return 1;
    }
    if ((board[fromRow][fromCol] == PAWN_WHITE || board[fromRow][fromCol] == QUEEN_WHITE) && toRow == fromRow - 2 &&
        (toCol == fromCol - 2 || toCol == fromCol + 2) &&
        board[fromRow - 1][fromCol - (fromCol - toCol) / 2] == PAWN_BLACK) {
        return 1;
    }
    if ((board[fromRow][fromCol] == PAWN_BLACK || board[fromRow][fromCol] == QUEEN_BLACK) && toRow == fromRow + 2 &&
        (toCol == fromCol - 2 || toCol == fromCol + 2) &&
        board[fromRow + 1][fromCol - (fromCol - toCol) / 2] == PAWN_WHITE) {
        return 1;
    }
    return 0;
}

void makeMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol) {
    if (abs(fromRow - toRow) == 2 || abs(fromRow - toRow) > 2) {
        int capturedRow = (fromRow + toRow) / 2;
        int capturedCol = (fromCol + toCol) / 2;
        board[capturedRow][capturedCol] = PAWN_NULL;
    }

    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = PAWN_NULL;

    if (toRow == 0 && board[toRow][toCol] == PAWN_WHITE) {
        board[toRow][toCol] = QUEEN_WHITE;
    } else if (toRow == NUM_CELL - 1 && board[toRow][toCol] == PAWN_BLACK) {
        board[toRow][toCol] = QUEEN_BLACK;
    }
}

void convertCoordinate(const char* coord, int* row, int* col) {
    *col = coord[0] - 'a';
    *row = coord[1] - '1';
}

int canCapture(PawnType board[NUM_CELL][NUM_CELL], int row, int col) {
    Move captureMoves[4];
    int moveCount = getCaptureMoves(board, row, col, captureMoves);
    return moveCount > 0;
}

int getCaptureMoves(PawnType board[NUM_CELL][NUM_CELL], int row, int col, Move captureMoves[]) {
    int moveCount = 0;

    if ((board[row][col] == PAWN_WHITE || board[row][col] == QUEEN_WHITE) && row > 1) {
        if (col > 1 && board[row - 1][col - 1] == PAWN_BLACK && board[row - 2][col - 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row - 2, col - 2, row - 1, col - 1, board[row][col]};
        }
        if (col < NUM_CELL - 2 && board[row - 1][col + 1] == PAWN_BLACK && board[row - 2][col + 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row - 2, col + 2, row - 1, col + 1, board[row][col]};
        }
    }

    if ((board[row][col] == PAWN_BLACK || board[row][col] == QUEEN_BLACK) && row < NUM_CELL - 2) {
        if (col > 1 && board[row + 1][col - 1] == PAWN_WHITE && board[row + 2][col - 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row + 2, col - 2, row + 1, col - 1, board[row][col]};
        }
        if (col < NUM_CELL - 2 && board[row + 1][col + 1] == PAWN_WHITE && board[row + 2][col + 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row + 2, col + 2, row + 1, col + 1, board[row][col]};
        }
    }

    if (board[row][col] == QUEEN_WHITE || board[row][col] == QUEEN_BLACK) {
        int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for (int d = 0; d < 4; d++) {
            int newRow = row + directions[d][0];
            int newCol = col + directions[d][1];
            int foundOpponent = 0;
            while (newRow >= 0 && newRow < NUM_CELL && newCol >= 0 && newCol < NUM_CELL) {
                if (board[newRow][newCol] == PAWN_NULL) {
                    if (foundOpponent) {
                        captureMoves[moveCount++] = (Move){row, col, newRow, newCol, newRow - directions[d][0], newCol - directions[d][1], board[row][col]};
                        break;
                    }
                } else if (board[newRow][newCol] % 2 != board[row][col] % 2) {
                    if (foundOpponent) break;
                    foundOpponent = 1;
                } else {
                    break;
                }
                newRow += directions[d][0];
                newCol += directions[d][1];
            }
        }
    }

    return moveCount;
}

int hasNoPawns(PawnType board[NUM_CELL][NUM_CELL], int player) {
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] % 2 == player % 2 && board[i][j] != PAWN_NULL) {
                return 0;
            }
        }
    }
    return 1;
}

int checkWinner(PawnType board[NUM_CELL][NUM_CELL]) {
    int whiteCount = 0, blackCount = 0;
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] == PAWN_WHITE || board[i][j] == QUEEN_WHITE) {
                whiteCount++;
            } else if (board[i][j] == PAWN_BLACK || board[i][j] == QUEEN_BLACK) {
                blackCount++;
            }
        }
    }
    if (whiteCount == 0) return PAWN_BLACK;
    if (blackCount == 0) return PAWN_WHITE;
    return PAWN_NULL;
}
