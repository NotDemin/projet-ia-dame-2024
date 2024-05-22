#ifndef GAME_H
#define GAME_H

#include "board.h"

#define SEARCH_DEPTH 5

typedef struct Node {
    Move move;
    struct Node* parent;
    struct Node* children[NUM_CELL * NUM_CELL];
    int childCount;
    int rating;
} Node;

typedef struct {
    Node* root;
} Tree;

Tree* createTree();
void addNode(Tree* tree, Move move, Node* parent);
Node* findBestMove(Tree* tree);
void freeTree(Tree* tree);

void checkLegalMoves(PawnType board[NUM_CELL][NUM_CELL], int row, int col, int curPlayer, Move moves[], int* moveCount);
int evaluateBoard(PawnType board[NUM_CELL][NUM_CELL]);
Move findBestMoveAI(PawnType board[NUM_CELL][NUM_CELL], int player);
int isGameOver(PawnType board[NUM_CELL][NUM_CELL]);
int minmax(PawnType board[NUM_CELL][NUM_CELL], int depth, int isMaximizingPlayer);

#endif // GAME_H
