#include "list.h"

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

#define MAX_BOARD 100
#define WH_BOARD 10
#define HG_BOARD 10

typedef enum {
    EMPTY = 0,
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2,
    MOVE_BOTH= 3,
    CAPTURE_LEFT = 4,
    CAPTURE_RIGHT = 5,
    CAPTURE_BOTH = 6
} MoveResult;7
struct isValidMove {
    int isValid;
    int count;
    list_t result;
};
typedef struct isValidMove isValidMove;

void initBoard(Item *node, char *board);
Item* initGame();

Item *getChildBoard( Item *node, int pos);

void printBoard( Item *board );
