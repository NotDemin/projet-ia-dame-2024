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

int isGameOver(PawnType board[NUM_CELL][NUM_CELL]) {
    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] == PAWN_WHITE || board[i][j] == QUEEN_WHITE) {
                Move moves[NUM_CELL * NUM_CELL];
                int moveCount = 0;
                checkLegalMoves(board, i, j, PAWN_WHITE, moves, &moveCount);
                if (moveCount > 0) {
                    return 0;
                }
            } else if (board[i][j] == PAWN_BLACK || board[i][j] == QUEEN_BLACK) {
                Move moves[NUM_CELL * NUM_CELL];
                int moveCount = 0;
                checkLegalMoves(board, i, j, PAWN_BLACK, moves, &moveCount);
                if (moveCount > 0) {
                    return 0;
                }
            }
        }
    }
}

int minmax(PawnType board[NUM_CELL][NUM_CELL], int depth, int maximizingPlayer, Move *bestMove) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    int bestRating = maximizingPlayer ? INT_MIN : INT_MAX;
    Move legalMoves[NUM_CELL * NUM_CELL];
    int moveCount = 0;

    for (int i = 0; i < NUM_CELL; i++) {
        for (int j = 0; j < NUM_CELL; j++) {
            if (board[i][j] != PAWN_NULL && board[i][j] % 2 == maximizingPlayer) {
                checkLegalMoves(board, i, j, maximizingPlayer, legalMoves, &moveCount);
            }
        }
    }

    for (int i = 0; i < moveCount; i++) {
        PawnType tempBoard[NUM_CELL][NUM_CELL];
        memcpy(tempBoard, board, sizeof(PawnType) * NUM_CELL * NUM_CELL);
        makeMove(tempBoard, legalMoves[i].row, legalMoves[i].col, legalMoves[i].toRow, legalMoves[i].toCol);

        int rating = minimax(tempBoard, depth - 1, !maximizingPlayer, NULL);

        if (maximizingPlayer) {
            if (rating > bestRating) {
                bestRating = rating;
                if (bestMove) {
                    *bestMove = legalMoves[i];
                }
            }
        } else {
            if (rating < bestRating) {
                bestRating = rating;
                if (bestMove) {
                    *bestMove = legalMoves[i];
                }
            }
        }
    }
    return bestRating;
}

Move findBestMoveAI(PawnType board[NUM_CELL][NUM_CELL], int player) {

    Move bestMove;
    minimax(board, SEARCH_DEPTH, player, &bestMove);
    return bestMove;
}
