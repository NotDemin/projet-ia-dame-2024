#include "list.h"

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

#define MAX_BOARD 100
#define WH_BOARD 10

void initBoard(Item *node, char *board);

Item *getChildBoard( Item *node, int pos, int isKnight);

void printBoard( Item *board );
