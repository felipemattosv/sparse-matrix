#include "node.h"
#include <stdlib.h>

Node *node_construct(double value, int line, int column, Node *nextOnLine, Node *nextOnColumn)
{
    Node *n = (Node *)malloc(1 * sizeof(Node));
    n->value = value;
    n->line = line;
    n->column = column;
    n->nextOnLine = nextOnLine;
    n->nextOnColumn = nextOnColumn;
    return n;
}

void node_destroy(Node *n)
{
    free(n);
}
