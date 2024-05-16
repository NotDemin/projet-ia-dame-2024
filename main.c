//
// Created by nathan on 19/04/24.
//

#include "board.h"
#include "list.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
//
//    initList(&openList_p);
//    initList(&closedList_p);

    Item *initial_state = initGame();
    printBoard(initial_state);

    return 0;
}