#include "game.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

Tree* createTree() {
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree->root = NULL;
    return tree;
}

void addNode(Tree* tree, Move move, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->move = move;
    node->parent = parent;
    node->childCount = 0;
    node->rating = 0;

    if (parent == NULL) {
        tree->root = node;
    } else {
        parent->children[parent->childCount++] = node;
    }
}

Node* findBestMove(Tree* tree) {
    Node* bestNode = NULL;
    int bestRating = INT_MIN;
    for (int i = 0; i < tree->root->childCount; i++) {
        if (tree->root->children[i]->rating > bestRating) {
            bestRating = tree->root->children[i]->rating;
            bestNode = tree->root->children[i];
        }
    }
    return bestNode;
}

void freeTree(Tree* tree) {
    if (tree->root != NULL) {
        for (int i = 0; i < tree->root->childCount; i++) {
            free(tree->root->children[i]);
        }
        free(tree->root);
    }
    free(tree);
}

void checkLegalMoves(PawnType board[NUM_CELL][NUM_CELL], int row, int col, int curPlayer, Move moves[], int* moveCount) {
    *moveCount = 0;
    // Ajoute les mouvements des pions
    if (board[row][col] == PAWN_WHITE || board[row][col] == PAWN_BLACK) {
        int direction = (board[row][col] == PAWN_WHITE) ? -1 : 1;
        if (row + direction >= 0 && row + direction < NUM_CELL) {
            if (col > 0 && board[row + direction][col - 1] == PAWN_NULL) {
                moves[(*moveCount)++] = (Move){row, col, row + direction, col - 1, -1, -1, board[row][col]};
            }
            if (col < NUM_CELL - 1 && board[row + direction][col + 1] == PAWN_NULL) {
                moves[(*moveCount)++] = (Move){row, col, row + direction, col + 1, -1, -1, board[row][col]};
            }
        }
    }

    // Ajoute les mouvements des reines
    if (board[row][col] == QUEEN_WHITE || board[row][col] == QUEEN_BLACK) {
        int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for (int i = 0; i < 4; i++) {
            int r = row + directions[i][0];
            int c = col + directions[i][1];
            while (r >= 0 && r < NUM_CELL && c >= 0 && c < NUM_CELL) {
                if (board[r][c] == PAWN_NULL) {
                    moves[(*moveCount)++] = (Move){row, col, r, c, -1, -1, board[row][col]};
                } else {
                    break;
                }
                r += directions[i][0];
                c += directions[i][1];
            }
        }
    }
}

int evaluateBoard(PawnType board[NUM_CELL][NUM_CELL]) {
    int score = 0;
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] == PAWN_WHITE) {
                score += 1;
            } else if (board[i][j] == PAWN_BLACK) {
                score -= 1;
            } else if (board[i][j] == QUEEN_WHITE) {
                score += 2;
            } else if (board[i][j] == QUEEN_BLACK) {
                score -= 2;
            }
        }
    }
    return score;
}

int minmax(PawnType board[NUM_CELL][NUM_CELL], int depth, int isMaximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    if (isMaximizingPlayer) {
        int bestValue = INT_MIN;
        for (int i = 0; i < NUM_CELL; i++) {
            for (int j = 0; j < NUM_CELL; j++) {
                if (board[i][j] == PAWN_WHITE || board[i][j] == QUEEN_WHITE) {
                    Move moves[NUM_CELL * NUM_CELL];
                    int moveCount = 0;
                    checkLegalMoves(board, i, j, PAWN_WHITE, moves, &moveCount);
                    for (int k = 0; k < moveCount; k++) {
                        PawnType tempBoard[NUM_CELL][NUM_CELL];
                        memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                        makeMove(tempBoard, moves[k].row, moves[k].col, moves[k].toRow, moves[k].toCol);
                        int value = minmax(tempBoard, depth - 1, 0);
                        bestValue = (value > bestValue) ? value : bestValue;
                    }
                }
            }
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        for (int i = 0; i < NUM_CELL; i++) {
            for (int j = 0; j < NUM_CELL; j++) {
                if (board[i][j] == PAWN_BLACK || board[i][j] == QUEEN_BLACK) {
                    Move moves[NUM_CELL * NUM_CELL];
                    int moveCount = 0;
                    checkLegalMoves(board, i, j, PAWN_BLACK, moves, &moveCount);
                    for (int k = 0; k < moveCount; k++) {
                        PawnType tempBoard[NUM_CELL][NUM_CELL];
                        memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                        makeMove(tempBoard, moves[k].row, moves[k].col, moves[k].toRow, moves[k].toCol);
                        int value = minmax(tempBoard, depth - 1, 1);
                        bestValue = (value < bestValue) ? value : bestValue;
                    }
                }
            }
        }
        return bestValue;
    }
}Move findBestMoveAI(PawnType board[NUM_CELL][NUM_CELL], int player) {
    Move captureMoves[NUM_CELL * NUM_CELL];
    int moveCount = 0;
    Move bestMove;
    int bestRating = (player == PAWN_WHITE) ? INT_MIN : INT_MAX;

    // Rechercher les mouvements de capture
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if ((player == PAWN_WHITE && (board[i][j] == PAWN_WHITE || board[i][j] == QUEEN_WHITE)) ||
                (player == PAWN_BLACK && (board[i][j] == PAWN_BLACK || board[i][j] == QUEEN_BLACK))) {
                int captures = getCaptureMoves(board, i, j, captureMoves);
                for (int k = 0; k < captures; k++) {
                    PawnType tempBoard[NUM_CELL][NUM_CELL];
                    memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                    makeMove(tempBoard, captureMoves[k].row, captureMoves[k].col, captureMoves[k].toRow, captureMoves[k].toCol);
                    int rating = evaluateBoard(tempBoard);
                    if ((player == PAWN_WHITE && rating > bestRating) || (player == PAWN_BLACK && rating < bestRating)) {
                        bestRating = rating;
                        bestMove = captureMoves[k];
                    }
                }
            }
        }
    }

    // Si aucun mouvement de capture n'est disponible, choisir un mouvement ordinaire
    if (moveCount == 0) {
        for (int i = 0; i < NUM_CELL; i++) {
            for (int j = 0; j < NUM_CELL; j++) {
                if ((player == PAWN_WHITE && (board[i][j] == PAWN_WHITE || board[i][j] == QUEEN_WHITE)) ||
                    (player == PAWN_BLACK && (board[i][j] == PAWN_BLACK || board[i][j] == QUEEN_BLACK))) {
                    Move moves[NUM_CELL * NUM_CELL];
                    int moveCount = 0;
                    checkLegalMoves(board, i, j, player, moves, &moveCount);
                    for (int k = 0; k < moveCount; k++) {
                        PawnType tempBoard[NUM_CELL][NUM_CELL];
                        memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                        makeMove(tempBoard, moves[k].row, moves[k].col, moves[k].toRow, moves[k].toCol);
                        int rating = evaluateBoard(tempBoard);
                        if ((player == PAWN_WHITE && rating > bestRating) || (player == PAWN_BLACK && rating < bestRating)) {
                            bestRating = rating;
                            bestMove = moves[k];
                        }
                    }
                }
            }
        }
    }

    return bestMove;
}
