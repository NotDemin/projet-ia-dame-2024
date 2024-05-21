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
        // Free nodes recursively
    }
    free(tree);
}

int evaluateBoard(PawnType board[NUM_CELL][NUM_CELL]) {
    int score = 0;
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] == PAWN_WHITE) {
                score += 1;
            } else if (board[i][j] == PAWN_BLACK) {
                score -= 1;
            } else if (board[i][j] == KING_WHITE) {
                score += 2;
            } else if (board[i][j] == KING_BLACK) {
                score -= 2;
            }
        }
    }
    return score;
}

int minimax(PawnType board[NUM_CELL][NUM_CELL], int depth, int isMaximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    int bestValue = isMaximizingPlayer ? INT_MIN : INT_MAX;

    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (isMaximizingPlayer && (board[i][j] == PAWN_WHITE || board[i][j] == KING_WHITE)) {
                for (int k = 0; k < NUM_CELL; k++) {
                    for (int l = 0; l < NUM_CELL; l++) {
                        if (isValidMove(board, i, j, k, l)) {
                            PawnType tempBoard[NUM_CELL][NUM_CELL];
                            memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                            makeMove(tempBoard, i, j, k, l);
                            int value = minimax(tempBoard, depth - 1, 0);
                            bestValue = bestValue > value ? bestValue : value;
                        }
                    }
                }
            } else if (!isMaximizingPlayer && (board[i][j] == PAWN_BLACK || board[i][j] == KING_BLACK)) {
                for (int k = 0; k < NUM_CELL; k++) {
                    for (int l = 0; l < NUM_CELL; l++) {
                        if (isValidMove(board, i, j, k, l)) {
                            PawnType tempBoard[NUM_CELL][NUM_CELL];
                            memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                            makeMove(tempBoard, i, j, k, l);
                            int value = minimax(tempBoard, depth - 1, 1);
                            bestValue = bestValue < value ? bestValue : value;
                        }
                    }
                }
            }
        }
    }

    return bestValue;
}

Move findBestMoveAI(PawnType board[NUM_CELL][NUM_CELL], int player) {
    Move bestMove;
    int bestRating = player == PAWN_WHITE ? INT_MIN : INT_MAX;

    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if ((player == PAWN_WHITE && (board[i][j] == PAWN_WHITE || board[i][j] == KING_WHITE)) ||
                (player == PAWN_BLACK && (board[i][j] == PAWN_BLACK || board[i][j] == KING_BLACK))) {
                for (int k = 0; k < NUM_CELL; k++) {
                    for (int l = 0; l < NUM_CELL; l++) {
                        if (isValidMove(board, i, j, k, l)) {
                            PawnType tempBoard[NUM_CELL][NUM_CELL];
                            memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
                            makeMove(tempBoard, i, j, k, l);
                            int rating = minimax(tempBoard, SEARCH_DEPTH, player == PAWN_BLACK);
                            if ((player == PAWN_WHITE && rating > bestRating) ||
                                (player == PAWN_BLACK && rating < bestRating)) {
                                bestRating = rating;
                                bestMove = (Move){i, j, k, l, -1, board[i][j]};
                            }
                        }
                    }
                }
            }
        }
    }

    return bestMove;
}
