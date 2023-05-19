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
    if (l < 0 || c < 0 || l >= m->numberOfLines || c >= m->numberOfColumns)
    {
        printf("Error: Position outside the bounds of the matrix!\n");
        return;
    }

    // If there is a node at position (l, c), destroy it and rearrange pointers
    if (v == 0)
    {
        if (matrix_get(m, l, c) == 0.0)
            return;

        Node *currentOnLine = m->linesHeads[l];
        Node *prevOnLine = NULL;

        while(currentOnLine->nextOnLine != NULL && currentOnLine->nextOnLine->column <= c)
        {
            prevOnLine = currentOnLine;
            currentOnLine = currentOnLine->nextOnLine;
        }

        if (currentOnLine->column == c)
        {
            if (prevOnLine != NULL)
                prevOnLine->nextOnLine = currentOnLine->nextOnLine;

            if (currentOnLine->column == m->linesHeads[l]->column)
            {
                m->linesHeads[l] = currentOnLine->nextOnLine;
            }

        }

        Node *currentOnColumn = m->columnsHeads[c];
        Node *prevOnColumn = NULL;

        while (currentOnColumn->nextOnColumn != NULL && currentOnColumn->nextOnColumn->line <= l)
        {
            prevOnColumn = currentOnColumn;
            currentOnColumn = currentOnColumn->nextOnColumn;
        }

        if (currentOnColumn->line == l)
        {   
            if (prevOnColumn != NULL)
                prevOnColumn->nextOnColumn = currentOnColumn->nextOnColumn;

            if (currentOnColumn->line == m->columnsHeads[c]->line)
            {
                m->columnsHeads[c] = currentOnColumn->nextOnColumn;
            }

            node_destroy(currentOnColumn);
        }
    }
    // Insert a node at position (l,c) with value 'v'
    else
    {
        Node *newNode = node_construct(v, l, c, NULL, NULL);

        // Insert the node in the forward list of line 'l'
        if (m->linesHeads[l] == NULL || c < m->linesHeads[l]->column)
        {
            newNode->nextOnLine = m->linesHeads[l];
            m->linesHeads[l] = newNode;
        }
        else
        {
            Node *current = m->linesHeads[l];
            while (current->nextOnLine != NULL && current->nextOnLine->column <= c)
            {
                current = current->nextOnLine;
            }
            if (current->column < c)
            {
                newNode->nextOnLine = current->nextOnLine;
                current->nextOnLine = newNode;
            }

            // There is already a node at position (l, c), so just changes its value
            // Note that it is not necessary to repeat this when iterating over the column as the node would be the same
            else if (current->column == c)
            {
                current->value = v;
                node_destroy(newNode);
            }
        }

        // Insert the node in the forward list of column 'c'
        if (m->columnsHeads[c] == NULL || l < m->columnsHeads[c]->line)
        {
            newNode->nextOnColumn = m->columnsHeads[c];
            m->columnsHeads[c] = newNode;
        }
        else
        {
            Node *current = m->columnsHeads[c];
            while (current->nextOnColumn != NULL && current->nextOnColumn->line <= l)
            {
                current = current->nextOnColumn;
            }
            
            if (current->line < l)
            {
            newNode->nextOnColumn = current->nextOnColumn;
                current->nextOnColumn = newNode; 
            }
        }
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
    if (a->numberOfLines != b->numberOfLines || a->numberOfColumns != b->numberOfColumns)
        exit(printf("Error: Cannot add matrices with different dimensions!\n"));

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
    if (a->numberOfLines != b->numberOfLines || a->numberOfColumns != b->numberOfColumns)
        exit(printf("Error: Cannot multiply element-wise two matrices with different dimensions!\n"));

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
    if (a->numberOfColumns != b->numberOfLines)
        exit(printf("Error: Cannot multiply matrices with these dimensions!\n"));

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

Matrix *matrix_slice(Matrix *m, int beginLine, int beginColumn, int endLine, int endColumn)
{
    Matrix *out = matrix_construct((endLine - beginLine) + 1, (endColumn - beginColumn) + 1);

    int countLine = beginLine;

    // Goes through all lines in the range
    for (int externalCount = 0; externalCount <= (endLine - beginLine); externalCount++)
    {    
        Node *current = m->linesHeads[countLine];

        // Finds the first node on the current line
        while (current != NULL && current->column < beginColumn)
        {
            current = current->nextOnLine;
        }

        // Loops through the nodes of the current line that are in the range, inserting them into the output matrix
        while (current != NULL && current->column <= endColumn)
        {
            matrix_insert(out, externalCount, (current->column - beginColumn), current->value);
            current = current->nextOnLine;
        }

        countLine++;
    }

    return out;
}

void matrix_swapLines(Matrix *m, int lineA, int lineB)
{
    if (lineA < 0 || lineB < 0 || lineA >= m->numberOfLines || lineB >= m->numberOfLines)
    {
        printf("Error: lines outside the matrix range!\n");
        return;
    }

    if (lineA == lineB)
        return;

    for (int countColumn = 0; countColumn < m->numberOfColumns; countColumn++)
    {   
        if (m->columnsHeads[countColumn] == NULL)
            continue; // If the column is composed only of zeros, go to next iteration

        Node *nA = m->columnsHeads[countColumn];
        Node *nB = m->columnsHeads[countColumn];

        // Get the node of lineA
        while (nA->nextOnColumn != NULL && nA->nextOnColumn->line <= lineA)
        {
            nA = nA->nextOnColumn;
        }
        
        // Get the node of lineB
        while (nB->nextOnColumn != NULL && nB->nextOnColumn->line <= lineB)
        {
            nB = nB->nextOnColumn;
        }

        if (nA->line == lineA && nB->line == lineB)
        {
            // Both nodes already exists, only change values
            double aux = nA->value;
            nA->value = nB->value;
            nB->value = aux;
        }
        else
        {
            if (nA->line == lineA && nB->line != lineB)
            {
                //Only nA already exists
                matrix_insert(m, lineB, countColumn, nA->value); //Insert a node in lineB
                matrix_insert(m, lineA, countColumn, 0); //Destroys the node in lineA
            }
            else if (nA->line != lineA && nB->line == lineB)
            {
                //Only nB already exists
                matrix_insert(m, lineA, countColumn, nB->value); //Insert a node in lineA
                matrix_insert(m, lineB, countColumn, 0); //Destroys the node in lineB
            }
        }
    }
}

void matrix_swapColumns(Matrix *m, int columnA, int columnB)
{
    if (columnA < 0 || columnB < 0 || columnA >= m->numberOfColumns || columnB >= m->numberOfColumns)
    {
        printf("Error: columns outside the matrix range!\n");
        return;
    }

    if (columnA == columnB)
        return;

    for (int countLine = 0; countLine < m->numberOfLines; countLine++)
    {
        if (m->linesHeads[countLine] == NULL)
            continue; // If the line is composed only of zeros, go to next iteration

        Node *nA = m->linesHeads[countLine];
        Node *nB = m->linesHeads[countLine];

        // Get the node of columnA
        while (nA->nextOnLine != NULL && nA->nextOnLine->column <= columnA)
        {
            nA = nA->nextOnLine;
        }

        // Get the node of columnB
        while (nB->nextOnLine != NULL && nB->nextOnLine->column <= columnB)
        {
            nB = nB->nextOnLine;
        }

        if (nA->column == columnA && nB->column == columnB)
        {
            // Both nodes already exists, only change values
            double aux = nA->value;
            nA->value = nB->value;
            nB->value = aux;
        }
        else
        {
            if (nA->column == columnA && nB->column != columnB)
            {
                // Only nA already exists
                matrix_insert(m, countLine, columnB, nA->value); //Insert a node in columnB
                matrix_insert(m, countLine, columnA, 0); //Destroys the node in columnA
            }
            else if (nA->column != columnA && nB->column == columnB)
            {
                // Only nB already exists
                matrix_insert(m, countLine, columnA, nB->value); //Insert a node in columnA
                matrix_insert(m, countLine, columnB, 0); //Destroys the node in columnB
            }
        }
    }
}

Matrix *matrix_transpose(Matrix *m)
{
    Matrix *t = matrix_construct(m->numberOfColumns, m->numberOfLines);

    // Goes throught each column, creating the lines of the transposed
    for (int countColumn=0; countColumn < m->numberOfColumns; countColumn++)
    {
        Node *current = m->columnsHeads[countColumn];

        while (current != NULL)
        {
            matrix_insert(t, countColumn, current->line, current->value);
            current = current->nextOnColumn;
        }
    }

    return t;
}
