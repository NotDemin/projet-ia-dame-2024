#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Item Functions */

int strcomp(char* s1, char* s2, int size)
{
    for (int i = 0; i < size; i++) {
        if (s1[i] != s2[i]) return 1;
    }
    return 0;
}

Item *newNode()
{
    Item *node;

    node = (Item *) malloc( sizeof(Item) );
    assert(node);

    node->board = NULL;
    node->parent = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->f = node->g = node->h = (double)0.0;

    return node;
}

void freeNode( Item *node )
{
    if (node && node->board) free(node->board);
    if (node) free(node);
}


void initList( list_t *list_p )
{
    // if (list_p == NULL) list_p = malloc(sizeof(list_t));
    assert(list_p);

    list_p->numElements = 0;
    list_p->first = NULL;
    list_p->last = NULL;
}

int listCount( list_t *list )
{
	return list->numElements;
}

// return an item with corresponding board , or null
Item* onList( list_t *list, char *board)
{
    Item *item = list->first;
    while (item) {
        if (strcomp(item->board, board, item->size) == 0) return item;
        item = item->next;
    }
    return NULL;
}

// remove a node from list
void delList( list_t *list, Item *node) {

    if (list == NULL || node == NULL)
        return;
    if (node == list->first)
    {
        list->first = node->next;
        if (list->first != NULL)
        {
            list->first->prev = NULL;
        }
    }
    else
    {
        node->prev->next = node->next;
    }
    if (node == list->last)
    {
        list->last = node->prev;
        if (list->last != NULL)
        {
            list->last->next = NULL;
        }
    }
    else
    {
        node->next->prev = node->prev;
    }
    list->numElements--;
    free(node);

}

// return and remove first item
Item *popFirst( list_t *list ) //
{
    if (list->numElements == 0)
    {
        Item *item = NULL;
        return item;
    }
    if (list->numElements == 1)
    {
        Item *item = list->first;
        list->first = NULL;
        list->last = NULL;
        list->numElements -= 1;
        return item;
    }

    Item *item = list->first;
    list->first = list->first->next;
    list->numElements -= 1;
    return item;
}

// return and remove last item
Item *popLast( list_t *list ) {
    if (list->numElements == 0)
    {
        Item *item = NULL;
        return item;
    }
    if (list->numElements == 1)
    {
        Item *item = list->last;
        list->first = NULL;
        list->last = NULL;
        list->numElements -= 1;
        return item;
    }
    Item *item = list->last;
    list->last = list->first->prev;
    list->numElements -= 1;
    return item;
}

// return and remove best item with minimal f value
Item *popBest( list_t *list ) // and remove the best board from the list.
{
    if (list->numElements == 0)
    {
        Item *item = NULL;
        return item;
    }
    if (list->numElements == 1)
    {
        Item *item = list->first;
        list->first = NULL;
        list->last = NULL;
        list->numElements -= 1;
        return item;
    }

    Item *item = list->first;
    Item *best = list->first;
    double min = list->first->f;
    while (item) {
        if (item->f < min) {
            min = item->f;
            best = item;
        }
        item = item->next;
    }

    if (best == list->first) {
        list->first = list->first->next;
        list->first->prev = NULL;
    } else if (best == list->last) {
        list->last = list->last->prev;
        list->last->next = NULL;
    } else {
        best->prev->next = best->next;
        best->next->prev = best->prev;
    }
    list->numElements -= 1;
    return best;
}

 // add item in top
void addFirst( list_t *list, Item *node ) // add in head
{

    if (list == NULL || node == NULL) return;

    if (list->numElements == 0 || list->first == NULL)
    {
        list->first = node;
        list->last = node;
        list->numElements = 1;
        node->prev = NULL;
        node->next = NULL;
    }
    else
    {
        node->next = list->first;
        node->prev = NULL;
        list->first->prev = node;
        list->first = node;
        list->numElements++;
    }
}

 // add item in queue
void addLast( list_t *list, Item *node ) // add in tail
{
    if (list == NULL || node == NULL) return;

    if (list->first == NULL && list->last == NULL)
    {
        list->first = node;
        list->last = node;
        node->next = NULL;
        node->prev = NULL;
        list->numElements++;
    }
    else
    {
        node->next = list->first;
        node->prev = NULL;
        list->first->prev = node;
        list->first = node;
        list->numElements++;
    }
}

void replaceBest( list_t *list, Item *item ) {
    if (list->numElements == 0) {
        addFirst(list, item);
        return;
    }

    Item *temp = onList(list, item->board);
    if (temp) {
        if (temp->f > item->f) {
            temp->f = item->f;
            temp->g = item->g;
            temp->h = item->h;
            temp->parent = item->parent;
        }
    } else {
        addLast(list, item);
    }
}

void cleanList( list_t *list )
{
    Item *item = list->first;
    while (item) {
        Item *next = item->next;
        freeNode(item);
        item = next;
    }
    list->first = list->last = NULL;
    list->numElements = 0;
}

void printList( list_t list ) {
    Item *item = list.first;
    while (item != NULL) {
      printf("%.2f [%s] - ", item->f, item->board);
      item = item->next;
    }
    printf(" (nb_items: %d)\n", list.numElements);
}



// TEST LIST
/*
int main()
{
    Item *item;
    char str[3];

    list_t openList;

    initList( &openList );

    for (int i=0; i<10; i++) {
        item = newNode();
        item->f = (int)i;
        sprintf(str, "%2d", i);
        item->board = strdup(str);
        addLast( &openList, item );
    }

    for (int i=20; i<25; i++) {
        item = newNode();
        item->f = (int)i;
        sprintf(str, "%2d", i);
        item->board = strdup(str);
        addFirst( &openList, item );
    }

    printList(openList);
    printf("\n");

    Item *node = popBest( &openList );
    printf("best node = %.2f\n", node->f);
    printList(openList);
    printf("\n");

    strcpy(str, "23");
    node = onList(&openList, str);
    if (node) printf("found %s: %.2f!\n", str, node->f);
    printList(openList);
    printf("\n");

//    node = popFirst(&openList);

    item = newNode();
    item->f = 50;
    sprintf(str, "50");
    item->board = strdup(str);
    addLast( &openList, item );
    printf("add 50 at the end\n");
    printList(openList);
    printf("\n");

    item = newNode();
    item->f = 20;
    sprintf(str, "20");
    item->board = strdup(str);
    addFirst( &openList, item );
    printf("add 20 at the beginning\n");
    printList(openList);
    printf("\n");

    node = popFirst(&openList);
    if (node) printf("pop first: %.2f\n", node->f);
    printList(openList);
    printf("\n");

    node = popLast(&openList);
    if (node) printf("pop last: %.2f\n", node->f);
    printList(openList);
    printf("\n");

    printf("clean\n");
    cleanList(&openList);
    printList(openList);
    printf("\n");

    return 0;
}
*/