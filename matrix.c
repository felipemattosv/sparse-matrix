#include "matrix.h"
#include "node.h"
#include <stdlib.h>

Matrix *matrix_construct(int numberOfLines, int numberOfColumns)
{
    Matrix *m = (Matrix *)malloc(1 * sizeof(Matrix));
    m->numberOfLines = numberOfLines;
    m->numberOfColumns = numberOfColumns;
    m->linesHeads = (Node **)calloc(numberOfLines, sizeof(Node *));
    m->columnsHeads = (Node **)calloc(numberOfColumns, sizeof(Node *));
    return m;
}

void matrix_destroy(Matrix *m)
{
    for (int y=0; y< m->numberOfLines; y++)
    {   
        Node *n = m->linesHeads[y];

        while (n != NULL)
        {
            Node *next = n->nextOnLine;
            node_destroy(n);
            n = next;
        }
    }
    free(m->linesHeads);
    free(m->columnsHeads);
    free(m);
}
