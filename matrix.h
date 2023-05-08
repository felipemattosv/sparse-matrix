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

/* Complexity: O(n*m);
Goes through each node destroying it. For that, uses 2 nested loops.
The number of cycles depends on the dimension (nXm) of the sparse matrix.
Note: The worst case occurs when all columns are filled with non-zeros.
*/
void matrix_destroy(Matrix *);

/* Complexity: O(m);
Uses a loop to go through all the nodes of the line until reaching the last one.
The number of cycles depends on the number of columns (m) of the matrix.
Note: The worst case occurs when all elements of the line are non-zeros.
*/
Node *matrix_getLastNodeOfLine(Matrix *, int);

/* Complexity: O(n);
Uses a loop to go through all the nodes of the column until reaching the last one.
The number of cycles depends on the number of lines (n) of the matrix.
Note: The worst case occurs when all elements of the column are non-zeros.
*/
Node *matrix_getLastNodeOfColumn(Matrix *, int);

/* Complexity: O(n*m);
Reads the matrix from the terminal, using 2 nested loops to go through each position and save non-zeros.
The number of cycles depends on the dimension (nXm) of the input matrix.
*/
Matrix *matrix_read();

#endif
