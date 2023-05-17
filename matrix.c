#include "matrix.h"
#include "node.h"
#include <stdlib.h>
#include <stdio.h>

Matrix *matrix_construct(int numberOfLines, int numberOfColumns)
{
    if (numberOfLines <= 0 || numberOfColumns <= 0)
        exit(printf("Fatal Error: You cannot create a matrix with these dimensions!\n"));

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
                matrix_insert(m, y, x, v);
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

void matrix_insert(Matrix *m, int l, int c, double v)
{
    if (!v)
    {
        printf("Error: You cannot insert a node with value equal to zero!\n");
        return;
    }

    if (l < 0 || l >= m->numberOfLines || c < 0 || c >= m->numberOfColumns)
    {
        printf("Error: Position outside the bounds of the matrix!\n");
        return;
    }

    Node *newNode = node_construct(v, l, c, NULL, NULL);

    // Insert the new node in the forward list of line l
    Node *n = m->linesHeads[l];
    Node *prevNode = NULL;
    
    while (n != NULL && n->column < c)
    {
        prevNode = n;
        n = n->nextOnLine;
    }
    
    if (prevNode == NULL)
    {
        // The new node will be the first in the list
        newNode->nextOnLine = n;
        m->linesHeads[l] = newNode;
    }
    else
    {
        // The new node will be inserted after prevNode
        newNode->nextOnLine = prevNode->nextOnLine;
        prevNode->nextOnLine = newNode;
    }

    // Insert the new node in the forward list of column c
    n = m->columnsHeads[c];
    prevNode = NULL;
    
    while (n != NULL && n->line < l)
    {
        prevNode = n;
        n = n->nextOnColumn;
    }

    if (prevNode == NULL)
    {
        // The new node will be the first in the list
        newNode->nextOnColumn = n;
        m->columnsHeads[c] = newNode;
    }
    else
    {
        // The new node will be inserted after prevNode
        newNode->nextOnColumn = prevNode->nextOnColumn;
        prevNode->nextOnColumn = newNode;
    }
}

double matrix_get(Matrix *m, int l, int c)
{
    if (l < 0 || l >= m->numberOfLines || c < 0 || c >= m->numberOfColumns)
        return 0;

    Node *n = m->linesHeads[l];
    Node *next = NULL;
    int found = 0;

    while (n != NULL && n->column <= c)
    {
        if (n->column == c)
        {
            found = 1;
            break;
        }

        next = n->nextOnLine;
        n = next;
    }

    if (found)
        return n->value;
    else
        return 0;
}

Matrix *matrix_multiplyByScalar(Matrix *in, double scalar)
{
    Matrix *out = matrix_construct(in->numberOfLines, in->numberOfColumns);

    for (int y=0; y < in->numberOfLines; y++)
    {
        Node *n = in->linesHeads[y];
        Node *next = NULL;

        while (n != NULL)
        {
            next = n->nextOnLine;
            matrix_insert(out, y, n->column, (n->value * scalar));
            n = next;
        }
    }

    return out;
}

Matrix *matrix_sum(Matrix *a, Matrix *b)
{
    Matrix *out = matrix_construct(a->numberOfLines, a->numberOfColumns);

    for (int y=0; y < a->numberOfLines; y++)
    {
        Node *nA = a->linesHeads[y];
        Node *nB = b->linesHeads[y];

        for (int x=0; x < a->numberOfColumns; x++)
        {
            if (nA == NULL && nB == NULL)
                break;

            double valueA = 0;
            double valueB = 0;

            if (nA != NULL && nA->column == x)
            {
                valueA = nA->value;
                nA = nA->nextOnLine;
            }

            if (nB != NULL && nB->column == x)
            {
                valueB = nB->value;
                nB = nB->nextOnLine;
            }

            double sum = valueA + valueB;
            if (sum != 0)
                matrix_insert(out, y, x, sum);
        }
    }
    return out;
}

Matrix *matrix_multipliesElementWise(Matrix *a, Matrix *b)
{
    Matrix *out = matrix_construct(a->numberOfLines, a->numberOfColumns);

    for (int y=0; y < a->numberOfLines; y++)
    {
        Node *nA = a->linesHeads[y];
        Node *nB = b->linesHeads[y];

        for (int x=0; x < a->numberOfColumns; x++)
        {
            if (nA == NULL && nB == NULL)
                break;

            double valueA = 0;
            double valueB = 0;

            if (nA != NULL && nA->column == x)
            {
                valueA = nA->value;
                nA = nA->nextOnLine;
            }

            if (nB != NULL && nB->column == x)
            {
                valueB = nB->value;
                nB = nB->nextOnLine;
            }

            double product = valueA * valueB;
            if (product != 0)
                matrix_insert(out, y, x, product);
        }
    }
    return out;
}

Matrix *matrix_multiply(Matrix *a, Matrix *b)
{
    Matrix *out = matrix_construct(a->numberOfLines, b->numberOfColumns);

    int countLine = 0;
    int countColumn = 0;

    // At each iteration, calculates the value of the resulting matrix element at the current position
    for (int externalCount=0; externalCount < (a->numberOfLines * b->numberOfColumns); externalCount++)
    {
        Node *nA = a->linesHeads[countLine];
        Node *nB = b->columnsHeads[countColumn];

        int internalCount = 0;
         double sum = 0;

        // Iterates over the columns of matrix 'a' and the lines of matrix 'b' simultaneously
        // multiplying and adding the corresponding elements
        while (internalCount < a->numberOfColumns)
        {
            double valueA = 0;
            double valueB = 0;

            if (nA != NULL && nA->column == internalCount)
            {
                valueA = nA->value;
                nA = nA->nextOnLine;
            }

            if (nB != NULL && nB->line == internalCount)
            {
                valueB = nB->value;
                nB = nB->nextOnColumn;
            }

            sum += valueA * valueB;
            
            internalCount++;
        }

        if (sum != 0)
            matrix_insert(out, countLine, countColumn, sum);

        // Update counters, checking if the iteration has reached the end of a line
        if (countColumn == (b->numberOfColumns - 1))
        {
            countColumn = 0;
            countLine++;
        }
        else {
            countColumn++;
        }
    }

    return out;
}
