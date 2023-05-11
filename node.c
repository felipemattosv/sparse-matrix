#include "node.h"
#include <stdlib.h>
#include <stdio.h>

Node *node_construct(double value, int line, int column, Node *nextOnLine, Node *nextOnColumn)
{
    if (line < 0 || column < 0)
        exit(printf("Fatal error: You cannot create a node in this position!\n"));

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
