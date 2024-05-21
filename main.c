#include <stdio.h>
#include "board.h"
#include "game.h"

int main() {
    PawnType board[NUM_CELL][NUM_CELL];
    initBoard(board);

    int curPlayer = PAWN_WHITE;
    char from[3], to[3];
    int fromRow, fromCol, toRow, toCol;

    while (1) {
        printBoard(board);

        if (curPlayer == PAWN_WHITE) {
            printf("White's turn. Enter move (e.g., a3 b4): ");
            scanf("%s %s", from, to);
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
            } else if (!isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                printf("Invalid move. Try again.\n");
                continue;
            }

            makeMove(board, fromRow, fromCol, toRow, toCol);
            curPlayer = PAWN_BLACK;
        } else {
            printf("Black's turn (AI)...\n");
            Move bestMove = findBestMoveAI(board, curPlayer);
            makeMove(board, bestMove.row, bestMove.col, bestMove.toRow, bestMove.toCol);
            curPlayer = PAWN_WHITE;
        }
    }

    return 0;
}
