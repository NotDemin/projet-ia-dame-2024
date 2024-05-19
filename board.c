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

int isValidPosition(Item* node, int pos) {
    int ii = pos / WH_BOARD;
    int jj = pos % WH_BOARD;
    int count = 0;
    if(!(jj = 0 || jj == WH_BOARD - 1 ))
    {
        if(node->board[(((ii+1)*WH_BOARD)+jj)+1] == 0)
        {
            count++;
        }
        if(node->board[(((ii+1)*WH_BOARD)+jj)-1] == 0)
        {
            count++;
        }
    }
    else
    {
        if(jj == 0)
        {
            if(node->board[(((ii+1)*WH_BOARD)+jj)+1] == 0)
            {
                count++;
            }
        }
        else
        {
            if(node->board[(((ii+1)*WH_BOARD)+jj)-1] == 0)
            {
                count++;
            }
        }
    }
    return count;
}

int isValidPositionQueen(Item* node, int pos) {
    int ii = pos / WH_BOARD;
    int jj = pos % WH_BOARD;

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

// Return a new item where a new queen is added at position pos if possible. NULL if not valid
Item *getChildBoard( Item *node, int pos, int isKnight) {

    // #TODO modif ça
    int isValid = 1;

    if (isValid) {
        // Allouer et initialiser le noeud enfant
        Item *child_p = newNode();
        if (child_p == NULL)
        {
            return NULL;
        }

        initBoard(child_p, node->board);

        child_p->board[pos] = 1;

        // Lier l'enfant à son parent
        child_p->parent = node;
        child_p->depth = node->depth + 1;

        child_p->prev = child_p->next = NULL;
        child_p->f = node->f + 1;
        child_p->g = child_p->h = 0.0;

        return child_p;
    }

    return NULL;
}