//
// Created by nathan on 19/04/24.
//

#include "board.h"
#include "list.h"
#include <stdio.h>

int MinMax(Item *node, int depth, int isMaximizingPlayer) {
    if (depth == 0) {
        return node->f;
    }

    if (isMaximizingPlayer) {
        int bestValue = -1000;
        for (int i = 0; i < MAX_BOARD; i++) {
            if (node->board[i] == 0) {
                Item *child = getChildBoard(node, i);
                if (child != NULL) {
                    int val = MinMax(child, depth - 1, 0);
                    bestValue = bestValue > val ? bestValue : val;
                }
            }
        }
        return bestValue;
    } else {
        int bestValue = 1000;
        for (int i = 0; i < MAX_BOARD; i++) {
            if (node->board[i] == 0) {
                Item *child = getChildBoard(node, i);
                if (child != NULL) {
                    int val = MinMax(child, depth - 1, 1);
                    bestValue = bestValue < val ? bestValue : val;
                }
            }
        }
        return bestValue;
    }
}

int main(int argc, char *argv[]) {
//
//    initList(&openList_p);
//    initList(&closedList_p);

    Item *initial_state = initGame();
    printBoard(initial_state);

    return 0;
}