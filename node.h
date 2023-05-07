#ifndef _NODE_H_
#define _NODE_H_

typedef struct Node
{
    double value;
    int line;
    int column;
    struct Node *nextOnLine;
    struct Node *nextOnColumn;
} Node;

#endif
