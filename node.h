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

/* Complexity: O(1);
Only memory allocation and assignments.
*/
Node *node_construct(double, int, int, Node *, Node *);

/* Complexity: O(1);
Memory free only.
*/
void node_destroy(Node *);

#endif
