#include "board.h"
#include <stdio.h>
#include <stdlib.h>

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
    printf("  a b c d e f g h i j\n");
    for (int i = 0; i < NUM_CELL; i++) {
        printf("%d ", i + 1);
        for (int j = 0; j < NUM_CELL; j++) {
            switch(board[i][j]) {
                case PAWN_NULL:
                    printf(". ");
                    break;
                case PAWN_WHITE:
                    printf("w ");
                    break;
                case PAWN_BLACK:
                    printf("b ");
                    break;
                case KING_WHITE:
                    printf("W ");
                    break;
                case KING_BLACK:
                    printf("B ");
                    break;
            }
        }
        printf("\n");
    }
}

int isValidMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol) {
    // Vérifiez que la destination est dans les limites du plateau
    if (toRow < 0 || toRow >= NUM_CELL || toCol < 0 || toCol >= NUM_CELL) {
        return 0;
    }

    // Vérifiez que la destination est vide
    if (board[toRow][toCol] != PAWN_NULL) {
        return 0;
    }

    // Vérifiez les mouvements valides pour les pions blancs
    if (board[fromRow][fromCol] == PAWN_WHITE && toRow == fromRow - 1 && (toCol == fromCol - 1 || toCol == fromCol + 1)) {
        return 1;
    }

    // Vérifiez les mouvements valides pour les pions noirs
    if (board[fromRow][fromCol] == PAWN_BLACK && toRow == fromRow + 1 && (toCol == fromCol - 1 || toCol == fromCol + 1)) {
        return 1;
    }

    // Ajoutez des vérifications pour les dames et les captures ici
    if ((board[fromRow][fromCol] == PAWN_WHITE || board[fromRow][fromCol] == KING_WHITE) && toRow == fromRow - 2 &&
        (toCol == fromCol - 2 || toCol == fromCol + 2) &&
        board[fromRow - 1][fromCol - (fromCol - toCol) / 2] == PAWN_BLACK) {
        return 1;
    }

    if ((board[fromRow][fromCol] == PAWN_BLACK || board[fromRow][fromCol] == KING_BLACK) && toRow == fromRow + 2 &&
        (toCol == fromCol - 2 || toCol == fromCol + 2) &&
        board[fromRow + 1][fromCol - (fromCol - toCol) / 2] == PAWN_WHITE) {
        return 1;
    }

    // Vérifiez les mouvements et captures valides pour les dames
    if (board[fromRow][fromCol] == KING_WHITE || board[fromRow][fromCol] == KING_BLACK) {
        int dirRow[] = {-1, -1, 1, 1};
        int dirCol[] = {-1, 1, -1, 1};
        for (int i = 0; i < 4; i++) {
            int r = fromRow + dirRow[i];
            int c = fromCol + dirCol[i];
            while (r >= 0 && r < NUM_CELL && c >= 0 && c < NUM_CELL) {
                if (board[r][c] != PAWN_NULL) break;
                if (r == toRow && c == toCol) return 1;
                r += dirRow[i];
                c += dirCol[i];
            }
        }
    }

    return 0;
}

void makeMove(PawnType board[NUM_CELL][NUM_CELL], int fromRow, int fromCol, int toRow, int toCol) {
    if (abs(fromRow - toRow) == 2) {
        int capturedRow = (fromRow + toRow) / 2;
        int capturedCol = (fromCol + toCol) / 2;
        board[capturedRow][capturedCol] = PAWN_NULL;
    }

    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = PAWN_NULL;

    // Promotion des pions à dames si atteignent le côté opposé
    if (board[toRow][toCol] == PAWN_WHITE && toRow == 0) {
        board[toRow][toCol] = KING_WHITE;
    } else if (board[toRow][toCol] == PAWN_BLACK && toRow == NUM_CELL - 1) {
        board[toRow][toCol] = KING_BLACK;
    }
}


void convertCoordinate(char* coord, int* row, int* col) {
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

    if ((board[row][col] == PAWN_WHITE || board[row][col] == KING_WHITE) && row > 1) {
        if (col > 1 && board[row - 1][col - 1] == PAWN_BLACK && board[row - 2][col - 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row - 2, col - 2, row - 1, col - 1, board[row][col]};
        }
        if (col < NUM_CELL - 2 && board[row - 1][col + 1] == PAWN_BLACK && board[row - 2][col + 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row - 2, col + 2, row - 1, col + 1, board[row][col]};
        }
    }

    if ((board[row][col] == PAWN_BLACK || board[row][col] == KING_BLACK) && row < NUM_CELL - 2) {
        if (col > 1 && board[row + 1][col - 1] == PAWN_WHITE && board[row + 2][col - 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row + 2, col - 2, row + 1, col - 1, board[row][col]};
        }
        if (col < NUM_CELL - 2 && board[row + 1][col + 1] == PAWN_WHITE && board[row + 2][col + 2] == PAWN_NULL) {
            captureMoves[moveCount++] = (Move){row, col, row + 2, col + 2, row + 1, col + 1, board[row][col]};
        }
    }

    // Ajout des captures pour les dames
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    if (board[row][col] == KING_WHITE || board[row][col] == KING_BLACK) {
        for (int i = 0; i < 4; i++) {
            int r = row + directions[i][0];
            int c = col + directions[i][1];
            while (r >= 0 && r < NUM_CELL && c >= 0 && c < NUM_CELL) {
                if (board[r][c] != PAWN_NULL) {
                    if ((board[row][col] == KING_WHITE && (board[r][c] == PAWN_BLACK || board[r][c] == KING_BLACK)) ||
                        (board[row][col] == KING_BLACK && (board[r][c] == PAWN_WHITE || board[r][c] == KING_WHITE))) {
                        int nextR = r + directions[i][0];
                        int nextC = c + directions[i][1];
                        if (nextR >= 0 && nextR < NUM_CELL && nextC >= 0 && nextC < NUM_CELL && board[nextR][nextC] == PAWN_NULL) {
                            captureMoves[moveCount++] = (Move){row, col, nextR, nextC, r, c, board[row][col]};
                        }
                    }
                    break;
                }
                r += directions[i][0];
                c += directions[i][1];
            }
        }
    }

    return moveCount;
}
