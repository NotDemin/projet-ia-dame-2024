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
    Move tempMoves[4];
    int captureCount = getCaptureMoves(board, row, col, tempMoves);

    if (captureCount > 0) {
        for (int i = 0; i < captureCount; i++) {
            moves[(*moveCount)++] = tempMoves[i];
        }
    } else {
        int directions[4][2] = {{1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
        for (int i = 0; i < 4; i++) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];
            if (isValidMove(board, row, col, newRow, newCol)) {
                moves[(*moveCount)++] = (Move){row, col, newRow, newCol, -1, -1, board[row][col]};
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
}

Move findBestMoveAI(PawnType board[NUM_CELL][NUM_CELL], int player) {
    Move bestMove;
    int bestRating = (player == PAWN_WHITE) ? INT_MIN : INT_MAX;

    // Rechercher les mouvements de capture
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if ((player == PAWN_WHITE && (board[i][j] == PAWN_WHITE || board[i][j] == QUEEN_WHITE)) ||
                (player == PAWN_BLACK && (board[i][j] == PAWN_BLACK || board[i][j] == QUEEN_BLACK))) {
                Move captureMoves[NUM_CELL * NUM_CELL];
                int captureCount = getCaptureMoves(board, i, j, captureMoves);
                for (int k = 0; k < captureCount; k++) {
                    PawnType tempBoard[NUM_CELL][NUM_CELL];
                    memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                    makeMove(tempBoard, captureMoves[k].row, captureMoves[k].col, captureMoves[k].toRow, captureMoves[k].toCol);
                    int rating = minmax(tempBoard, SEARCH_DEPTH, player == PAWN_WHITE ? 0 : 1);
                    if ((player == PAWN_WHITE && rating > bestRating) ||
                        (player == PAWN_BLACK && rating < bestRating)) {
                        bestRating = rating;
                        bestMove = captureMoves[k];
                    }
                }
            }
        }
    }

    // Si aucun mouvement de capture n'est disponible, choisir un mouvement ordinaire
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
                    int rating = minmax(tempBoard, SEARCH_DEPTH, player == PAWN_WHITE ? 0 : 1);
                    if ((player == PAWN_WHITE && rating > bestRating) ||
                        (player == PAWN_BLACK && rating < bestRating)) {
                        bestRating = rating;
                        bestMove = moves[k];
                    }
                }
            }
        }
    }

    return bestMove;
}