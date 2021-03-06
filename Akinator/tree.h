#include <stdlib.h>
#include <errno.h>
#define MAX_LENGTH 50
enum error
{
    NULLPTR               = 10001001,
    BRKN                  = 10001100,
    EMPTY                 = 10001011,
    SAVEERR               = 10001100,
    NOCHAR                = 10001101,

};
enum side
{
    LEFT = 1,
    RIGHT = 2,
    NONE = 3,
};

typedef char* elem_t;

typedef struct node node_t;

struct node
{
    elem_t elem;

    node_t* parent;
    node_t* left;
    node_t* right;
};

typedef struct tree
{
    node_t* root;
    int size;

} tree_t;

node_t* nodeCtor(tree_t* s, node_t* parent, elem_t value);
int nodeOK(node_t* node, node_t* parent);
int treeOK(tree_t *s);
int treeDump (int value, tree_t *s);
int nodeDelete(tree_t* s, node_t** parent);
int treeDtor(tree_t** s);
tree_t* treeCtor(tree_t* s);
node_t* nodePushRoot(tree_t* s, elem_t value);
node_t* nodePush(tree_t* s, node_t* parent, int side, elem_t value);
node_t* nodeValChange(tree_t* s, node_t* node, elem_t value);
node_t* nodeNext(node_t* node, int side);
node_t* nodeParent(node_t* node, int side);
node_t* nodeRoot(tree_t* s);
node_t* nodeLeft(tree_t* s);
node_t* nodeRight(tree_t* s);
void nodeShow(FILE* out, const node_t* cur, const int parent, int* number, int side);
void treeShow(tree_t* s);
