#include "matrix.h"
#include "node.h"
#include <stdlib.h>
#include <stdio.h>

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

Node *matrix_getLastNodeOfLine(Matrix *m, int line)
{
    Node *n = m->linesHeads[line];
    
    while (n->nextOnLine != NULL)
        n = n->nextOnLine;

    return n;
}

Node *matrix_getLastNodeOfColumn(Matrix *m, int column)
{
    Node *n = m->columnsHeads[column];

    while (n->nextOnColumn != NULL)
        n = n->nextOnColumn;

    return n;
}

Matrix *matrix_read()
{
    int numberOfLines=0, numberOfColumns=0;
    double v=0.0;

    printf("Number of lines:");
    scanf("%d", &numberOfLines);
    printf("Number of columns:");
    scanf("%d", &numberOfColumns);

    Matrix *m = matrix_construct(numberOfLines, numberOfColumns);

    printf("Enter the matrix:\n");

    for (int y=0; y < numberOfLines; y++)
    {
        for (int x=0; x < numberOfColumns; x++)
        {
            scanf("%lf", &v);
            if (v != 0.0)
            {
                Node *n = node_construct(v, y, x, NULL, NULL);

                if (m->linesHeads[y] == NULL)
                {
                    // Set this node as the line 'y' head
                    m->linesHeads[y] = n;
                
                } else {
                    // Set the 'nextOnLine' prop of the last node of the line 'y' as this node
                    Node *prevOnLine = matrix_getLastNodeOfLine(m, y);
                    prevOnLine->nextOnLine = n;
                }

                if (m->columnsHeads[x] == NULL)
                {
                    // Set this node as the column 'x' head
                    m->columnsHeads[x] = n;

                } else {
                    //Set the 'nextOnColumn' prop of the last node of the column 'x' as this node
                    Node *prevOnColumn = matrix_getLastNodeOfColumn(m, x);
                    prevOnColumn->nextOnColumn = n;
                }
            }
        }
    }
    return m;
}

void matrix_showSparse(Matrix *m)
{
    for (int y=0; y < m->numberOfLines; y++)
    {   
        Node *n = m->linesHeads[y];

        while (n != NULL)
        {
            printf("[%d, %d]: %.2lf\n", n->line, n->column, n->value);
            n = n->nextOnLine;
        }
    }
}

void matrix_showDense(Matrix *m)
{
    for (int y=0; y < m->numberOfLines; y++)
    {
        Node *n = m->linesHeads[y];

        if (n == NULL)
        {
            // line is empty
            for (int x=0; x < m->numberOfColumns; x++)
            {
                printf("0.00");
                if (x < m->numberOfColumns -1)
                    printf(" ");
            }
        }
        else
        {
            // line is not empty
            for (int x=0; x < m->numberOfColumns; x++)
            {
                if (n->column == x)
                {
                    printf("%.2lf", n->value);
                    if (n->nextOnLine != NULL)
                        n = n->nextOnLine;
                
                } else
                    printf("0.00");

                if (x < m->numberOfColumns -1)
                    printf(" ");
            }
        }
        printf("\n");
    }
}
