#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "node.h"

typedef struct
{
    int numberOfLines;
    int numberOfColumns;
    Node **linesHeads;
    Node **columnsHeads;
} Matrix;

/* Complexity: O(1);
Only memory allocation and assignments.
*/
Matrix *matrix_construct(int, int);

/* Complexity: O(N);
Goes through each node destroying it.
The number of cycles depends on the number of nodes (N) on the matrix.
*/
void matrix_destroy(Matrix *);

/* Complexity: O(N);
Uses a loop to reach the last node of a line and return it.
The number of cycles depends on the number of nodes (N) in the line.
*/
Node *matrix_getLastNodeOfLine(Matrix *, int);

/* Complexity: O(N);
Uses a loop to reach the last node of a column and return it.
The number of cycles depends on the number of nodes (N) in the column.
*/
Node *matrix_getLastNodeOfColumn(Matrix *, int);

/* Complexity: O(n*m);
Reads the matrix from the terminal, using 2 nested loops to go through each position and save non-zeros.
The number of cycles depends on the dimension (nXm) of the input matrix.
*/
Matrix *matrix_read();

#endif
