#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "board.h"
#include <string.h>

// initialisation du jeu de dames
Item* initGame()
{
    Item *node;

    char *initial = (char*)malloc(MAX_BOARD*sizeof(char));
    for (int i=0; i<MAX_BOARD; i++) initial[i] = 0;

    // boucle pour placé les pions des deux couleurs
    for (int i = 0; i < MAX_BOARD; i++)
    {
        int x = i / WH_BOARD;
        int y = i % WH_BOARD;

        // initialisation des cases vides par défaut
        initial[i] = 0;

        // initialisation des pions noirs
        if (x <= 3)
        {
            if ((x + y) % 2 == 0)
            {
                initial[i] = 1;
            }
        }

        // initialisation des pions blancs
        if (x >= 6)
        {
            if ((x + y) % 2 == 0)
            {
                initial[i] = 2;
            }
        }

    }

    node = newNode();
    initBoard(node, initial);

    node->depth = 0;

    return node;
}


// print a board
void printBoard( Item *node )
{
    assert(node);
    printf("\n");
    for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
    for (int i = 0 ; i < MAX_BOARD ; i++) {
        if (i%WH_BOARD == 0) printf("|");
        if (node->board[i] == 0) printf("   |");
        else printf(" %d |", node->board[i]);
        if (((i+1)%WH_BOARD) == 0) {
            printf("\n");
            for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
        }
    }
    printf("\n");
}

isValidMove* isValidPosition(Item* node, int pos) {
    int ii = pos / WH_BOARD; // Calcul de la ligne
    int jj = pos % WH_BOARD; // Calcul de la colonne
    isValidMove *result = (isValidMove*)malloc(sizeof(isValidMove));
    initList(result->result);
    Item *itemResult = newNode();

    result->count = 0;

    // Vérifier si la case contient un pion blanc ou noir
    char piece = node->board[ii * WH_BOARD + jj];
    if (piece != 1 && piece != 2) {
        return 0; // La case ne contient pas un pion valide
    }

    int direction = (piece == 1) ? -1 : 1; // Direction de déplacement : blanc (-1) vers le haut, noir (+1) vers le bas

    // Vérifier les déplacements et captures diagonales
    if (jj > 0 && jj < WH_BOARD - 1) {
        // Vérifier si les cases diagonales droite et gauche sont vides
        if (ii + direction >= 0 && ii + direction < WH_BOARD) {
            if (node->board[(ii + direction) * WH_BOARD + (jj + 1)] == 0) {
                result->count++;
                itemResult->f = MOVE_RIGHT;
                addLast(result->result, itemResult);
            }
            if (node->board[(ii + direction) * WH_BOARD + (jj - 1)] == 0) {
                result->count++;
                itemResult->f = MOVE_LEFT;
                addLast(result->result, itemResult);
            }
        }

        // Vérifier les captures pour un pion blanc
        if (piece == 1) {
            if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                (jj + 2 < WH_BOARD) &&
                node->board[(ii + direction) * WH_BOARD + (jj + 1)] == 2 &&
                node->board[(ii + 2 * direction) * WH_BOARD + (jj + 2)] == 0) {
                result->count += 2;
                itemResult->f = CAPTURE_RIGHT;
                addLast(result->result, itemResult);
            }
            if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                (jj - 2 >= 0) &&
                node->board[(ii + direction) * WH_BOARD + (jj - 1)] == 2 &&
                node->board[(ii + 2 * direction) * WH_BOARD + (jj - 2)] == 0) {
                result->count += 2;
                itemResult->f = CAPTURE_LEFT;
                addLast(result->result, itemResult);
            }
        }

        // Vérifier les captures pour un pion noir
        if (piece == 2) {
            if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                (jj + 2 < WH_BOARD) &&
                node->board[(ii + direction) * WH_BOARD + (jj + 1)] == 1 &&
                node->board[(ii + 2 * direction) * WH_BOARD + (jj + 2)] == 0) {
                result->count += 2;
                itemResult->f = CAPTURE_RIGHT;
                addLast(result->result, itemResult);
            }
            if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                (jj - 2 >= 0) &&
                node->board[(ii + direction) * WH_BOARD + (jj - 1)] == 1 &&
                node->board[(ii + 2 * direction) * WH_BOARD + (jj - 2)] == 0) {
                result->count += 2;
                itemResult->f = CAPTURE_LEFT;
                addLast(result->result, itemResult);
            }
        }
    } else {
        // Vérifier les limites de la colonne gauche
        if (jj == 0 && ii + direction >= 0 && ii + direction < WH_BOARD) {
            if (node->board[(ii + direction) * WH_BOARD + (jj + 1)] == 0) {
                result->count++;
            }

            // Vérifier les captures pour un pion blanc
            if (piece == 1) {
                if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                    node->board[(ii + direction) * WH_BOARD + (jj + 1)] == 2 &&
                    node->board[(ii + 2 * direction) * WH_BOARD + (jj + 2)] == 0) {
                    result->count += 2;
                    itemResult->f = CAPTURE_RIGHT;
                    addLast(result->result, itemResult);
                }
            }

            // Vérifier les captures pour un pion noir
            if (piece == 2) {
                if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                    node->board[(ii + direction) * WH_BOARD + (jj + 1)] == 1 &&
                    node->board[(ii + 2 * direction) * WH_BOARD + (jj + 2)] == 0) {
                    result->count += 2;
                    itemResult->f = CAPTURE_RIGHT;
                    addLast(result->result, itemResult);
                }
            }
        } else if (jj == WH_BOARD - 1 && ii + direction >= 0 && ii + direction < WH_BOARD) { // Vérifier les limites de la colonne droite
            if (node->board[(ii + direction) * WH_BOARD + (jj - 1)] == 0) {
                count++;
            }

            // Vérifier les captures pour un pion blanc
            if (piece == 1) {
                if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                    node->board[(ii + direction) * WH_BOARD + (jj - 1)] == 2 &&
                    node->board[(ii + 2 * direction) * WH_BOARD + (jj - 2)] == 0) {
                    count += 2;
                    itemResult->f = CAPTURE_LEFT;
                    addLast(result->result, itemResult);
                }
            }

            // Vérifier les captures pour un pion noir
            if (piece == 2) {
                if ((ii + 2 * direction >= 0) && (ii + 2 * direction < WH_BOARD) &&
                    node->board[(ii + direction) * WH_BOARD + (jj - 1)] == 1 &&
                    node->board[(ii + 2 * direction) * WH_BOARD + (jj - 2)] == 0) {
                    count += 2;
                    itemResult->f = CAPTURE_LEFT;
                    addLast(result->result, itemResult);
                }
            }
        }
    }

    result->count = count;
    result->isValid = (count > 0) ? 1 : 0;

    return result;
}


int isValidPositionQueen(Item* node, int pos) {
    int ii = pos / WH_BOARD;
    int jj = pos % WH_BOARD;
    int count = 0;
    // à modifier
    for (int i=0; i<WH_BOARD; i++)
    {
        for (int j=0; j<WH_BOARD; j++)
        {
            if((abs(ii-i) == abs( jj-j))){
                if (node->board[i*WH_BOARD+j]!=0)
                {
                    return 1;
                }
            }

        }
    }
    return 0;
}

// initialize node's state from a given board
void initBoard(Item *node, char *board) {
	assert( node );

	node->size = MAX_BOARD;
    node->board = calloc(MAX_BOARD, sizeof(char));

    memcpy(node->board, board, MAX_BOARD * sizeof(char));
}

int MinMax(Item *node, int depth, int isMaximizingPlayer) {
    if (depth == 0) {
        return node->f;
    }

    if (isMaximizingPlayer) {
        int bestValue = -1000;
        for (int i = 0; i < MAX_BOARD; i++) {
            if (node->board[i] == 0) {
                Item *child = getChildBoard(node, i, 0);
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
                Item *child = getChildBoard(node, i, 0);
                if (child != NULL) {
                    int val = MinMax(child, depth - 1, 1);
                    bestValue = bestValue < val ? bestValue : val;
                }
            }
        }
        return bestValue;
    }
}

// Return a new item where a new queen is added at position pos if possible. NULL if not valid
Item *getChildBoard( Item *node, int pos) {

    // #TODO modif ça
    isValidMove *result = isValidPosition(node, pos);

    if (result->isValid) {
        // on récup le meilleur prochain coup avec minmax
        for (int i = 0; i < MAX_BOARD; i++) {
            if (node->board[i] == 0) {
                Item *child = newNode();
                initBoard(child, node->board);
                child->board[i] = 1;
                child->depth = node->depth + 1;
                child->f = MinMax(child, 3, 0);
                return child;
            }
        }
    }


    return NULL;
}