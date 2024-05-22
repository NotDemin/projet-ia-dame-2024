#include "board.h"
#include "game.h"
#include <stdio.h>

void getPlayerMove(char* from, char* to) {
    printf("White's turn. Enter move (e.g., a3 b4): ");
    scanf("%s %s", from, to);
}

void handleAIMove(PawnType board[NUM_CELL][NUM_CELL], int curPlayer) {
    printf("%s's turn (AI)...\n", curPlayer == PAWN_WHITE ? "White" : "Black");
    Move bestMove = findBestMoveAI(board, curPlayer);
    makeMove(board, bestMove.row, bestMove.col, bestMove.toRow, bestMove.toCol);
}

int main() {
    PawnType board[NUM_CELL][NUM_CELL];
    initBoard(board);

    int gameMode;
    printf("Choisissez le mode de jeu:\n1. Joueur contre IA\n2. IA contre IA\n");
    scanf("%d", &gameMode);

    int curPlayer = PAWN_WHITE;
    char from[3], to[3];
    int fromRow, fromCol, toRow, toCol;

    while (checkWinner(board) == PAWN_NULL) {
        printBoard(board);

        if (curPlayer == PAWN_WHITE && gameMode == 1) {
            getPlayerMove(from, to);
            convertCoordinate(from, &fromRow, &fromCol);
            convertCoordinate(to, &toRow, &toCol);

            if (canCapture(board, fromRow, fromCol)) {
                Move captureMoves[NUM_CELL * NUM_CELL];
                int captureCount = getCaptureMoves(board, fromRow, fromCol, captureMoves);
                int validCapture = 0;
                for (int i = 0; i < captureCount; i++) {
                    if (captureMoves[i].toRow == toRow && captureMoves[i].toCol == toCol) {
                        validCapture = 1;
                        break;
                    }
                }
                if (!validCapture) {
                    printf("You must capture the available piece(s).\n");
                    continue;
                }
            } else {
                int anyCapture = 0;
                for (int i = 0; i < NUM_CELL; i++) {
                    for (int j = 0; j < NUM_CELL; j++) {
                        if (board[i][j] == PAWN_WHITE && canCapture(board, i, j)) {
                            anyCapture = 1;
                            break;
                        }
                    }
                    if (anyCapture) break;
                }

                if (anyCapture) {
                    printf("You must capture the available piece(s).\n");
                    continue;
                }

                if (!isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                    printf("Invalid move. Try again.\n");
                    continue;
                }
            }

            makeMove(board, fromRow, fromCol, toRow, toCol);
        } else {
            handleAIMove(board, curPlayer);
        }

        curPlayer = (curPlayer == PAWN_WHITE) ? PAWN_BLACK : PAWN_WHITE;
    }


    printBoard(board);
    printf("Le gagnant est : %s\n", checkWinner(board) == PAWN_WHITE ? "Blanc" : "Noir");

    return 0;
}
