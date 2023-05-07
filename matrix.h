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

#endif
