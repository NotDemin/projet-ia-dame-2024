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
        printf("%2d ", i);
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

    int rowDiff = abs(fromRow - toRow);
    int colDiff = abs(fromCol - toCol);

    // Vérifier les mouvements diagonaux pour les pions
    if (rowDiff == 1 && colDiff == 1) {
        if (board[fromRow][fromCol] == PAWN_WHITE && toRow < fromRow) {
            return 1;
        }
        if (board[fromRow][fromCol] == PAWN_BLACK && toRow > fromRow) {
            return 1;
        }
    }

    // Vérifier les mouvements diagonaux pour les reines
    if (board[fromRow][fromCol] == QUEEN_WHITE || board[fromRow][fromCol] == QUEEN_BLACK) {
        if (rowDiff == colDiff) {
            int rowDirection = (toRow - fromRow) / rowDiff;
            int colDirection = (toCol - fromCol) / colDiff;
            for (int i = 1; i < rowDiff; i++) {
                if (board[fromRow + i * rowDirection][fromCol + i * colDirection] != PAWN_NULL) {
                    return 0;
                }
            }
            return 1;
        }
    }

    // Vérifier les captures pour les pions
    if (rowDiff == 2 && colDiff == 2) {
        if ((board[fromRow][fromCol] == PAWN_WHITE && toRow < fromRow) ||
            (board[fromRow][fromCol] == PAWN_BLACK && toRow > fromRow)) {
            int middleRow = (fromRow + toRow) / 2;
            int middleCol = (fromCol + toCol) / 2;
            if ((board[middleRow][middleCol] == PAWN_WHITE || board[middleRow][middleCol] == QUEEN_WHITE) &&
                board[fromRow][fromCol] == PAWN_BLACK) {
                return 1;
            }
            if ((board[middleRow][middleCol] == PAWN_BLACK || board[middleRow][middleCol] == QUEEN_BLACK) &&
                board[fromRow][fromCol] == PAWN_WHITE) {
                return 1;
            }
        }
    }

    // Vérifier les captures pour les reines
    if (board[fromRow][fromCol] == QUEEN_WHITE || board[fromRow][fromCol] == QUEEN_BLACK) {
        if (rowDiff == colDiff) {
            int rowDirection = (toRow - fromRow) / rowDiff;
            int colDirection = (toCol - fromCol) / colDiff;
            int foundOpponent = 0;
            for (int i = 1; i < rowDiff; i++) {
                int currRow = fromRow + i * rowDirection;
                int currCol = fromCol + i * colDirection;
                if (board[currRow][currCol] != PAWN_NULL) {
                    if (board[currRow][currCol] % 2 != board[fromRow][fromCol] % 2) {
                        if (foundOpponent) {
                            return 0;
                        }
                        foundOpponent = 1;
                    } else {
                        return 0;
                    }
                }
            }
            return foundOpponent;
        }
    }

    return 0;
}

void makeMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol) {
    if (abs(fromRow - toRow) == 2 || abs(fromRow - toRow) > 2) {
        int capturedRow = (fromRow + toRow) / 2;
        int capturedCol = (fromCol + toCol) / 2;

        // Cas où une reine capture sur une distance plus longue
        if (board[fromRow][fromCol] == QUEEN_WHITE || board[fromRow][fromCol] == QUEEN_BLACK) {
            int rowDirection = (toRow > fromRow) ? 1 : -1;
            int colDirection = (toCol > fromCol) ? 1 : -1;
            capturedRow = fromRow + rowDirection;
            capturedCol = fromCol + colDirection;
            while (capturedRow != toRow && capturedCol != toCol) {
                if (board[capturedRow][capturedCol] != PAWN_NULL && board[capturedRow][capturedCol] % 2 != board[fromRow][fromCol] % 2) {
                    board[capturedRow][capturedCol] = PAWN_NULL;
                    break;
                }
                capturedRow += rowDirection;
                capturedCol += colDirection;
            }
        } else {
            board[capturedRow][capturedCol] = PAWN_NULL;
        }
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
    *row = coord[1] - '0';
}

int canCapture(PawnType board[NUM_CELL][NUM_CELL], int row, int col) {
    Move captureMoves[4];
    int moveCount = getCaptureMoves(board, row, col, captureMoves);
    return moveCount > 0;
}

int getCaptureMoves(PawnType board[NUM_CELL][NUM_CELL], int row, int col, Move captureMoves[]) {
    int moveCount = 0;

    // Vérifier les captures pour les pions
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

    // Vérifier les captures pour les reines
    if (board[row][col] == QUEEN_WHITE || board[row][col] == QUEEN_BLACK) {
        int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for (int d = 0; d < 4; d++) {
            int rowDir = directions[d][0];
            int colDir = directions[d][1];
            int newRow = row + rowDir;
            int newCol = col + colDir;
            int foundOpponent = 0;
            while (newRow >= 0 && newRow < NUM_CELL && newCol >= 0 && newCol < NUM_CELL) {
                if (board[newRow][newCol] != PAWN_NULL) {
                    if (board[newRow][newCol] % 2 != board[row][col] % 2) {
                        foundOpponent = 1;
                    } else {
                        break;
                    }
                } else {
                    if (foundOpponent) {
                        captureMoves[moveCount++] = (Move){row, col, newRow, newCol, newRow - rowDir, newCol - colDir, board[row][col]};
                        break;
                    }
                }
                newRow += rowDir;
                newCol += colDir;
            }
        }
    }

    return moveCount;
}

/*int hasNoPawns(PawnType board[NUM_CELL][NUM_CELL], int player) {
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] % 2 == player % 2 && board[i][j] != PAWN_NULL) {
                return 0;
            }
        }
    }
    return 1;
}
*/
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
