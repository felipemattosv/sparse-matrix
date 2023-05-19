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

/* Complexity: O(N*n+N*m);
Reads the matrix from the terminal, using 2 nested loops to go through each position and save non-zeros.
To save, uses the insert function.
In short, does 'N' inserts (each one with complexity O(n+m)), where N is the number of non-zeros on the input matrix.
*/
Matrix *matrix_read();

/* Complexity: O(N);
Goes through all the nodes of the matrix and prints their position and value.
The number of cycles depends on the number of nodes (N) in the matrix.
*/
void matrix_showSparse(Matrix *);

/* Complexity: O(n*m);
Uses 2 nested loops to go through each position and print its value.
The number of cycles depends on the dimension (nXm) of the output matrix.
*/
void matrix_showDense(Matrix *);

/* Complexity: O(n+m);
Inserts a value at a given position.
First, it inserts the node in the line's list, with complexity O(n), where 'n' is the number of nodes in the line.
Then, inserts the node in the column's list, with complexity O(m), where 'm' is the number of nodes in the column.
*/
void matrix_insert(Matrix *, int, int, double);

/* Complexity: O(N);
Returns the value at a given position.
First accesses the head of the line, then iterates to the searched column.
The number of cycles depends on the number of nodes (N) on the searched line.
*/
double matrix_get(Matrix *, int, int);

/* Complexity: O(N*n+N*m);
Returns a new matrix obtained by multiplying the input matrix by the given scalar.
For that, uses 2 nested loops and for each node in the input matrix, inserts a node in the output matrix.
In short, does 'N' inserts (each one with complexity O(n+m)), where N is the number of nodes on the input matrix.
*/
Matrix *matrix_multiplyByScalar(Matrix *, double);

/* Complexity: O(q*n+q*m);
Returns a new matrix obtained by adding the two input matrices.
For each position where the sum is different from zero, inserts an element in the output matrix.
In short, does 'q' inserts (each one with complexity O(n+m)), where 'q' is the number of non-zero sums.
*/
Matrix *matrix_sum(Matrix *, Matrix *);

/* Complexity: O(q*n+q*m);
Returns a new matrix obtained by element-wise multiplication between the input matrices.
For each position where the product is different from zero, inserts an element in the output matrix.
In short, does 'q' inserts (each one with complexity O(n+m)), where 'q' is the number of non-zero products.
*/
Matrix *matrix_multipliesElementWise(Matrix *, Matrix *);

/* Complexity: O(q*n+q*m);
Returns a new matrix obtained by the multiplication between the input matrices.
For each non-zero element obtained, inserts an element in the output matrix.
In short, does 'q' inserts (each one with complexity O(n+m)), where 'q' is the number of non-zeros at the output matrix.
*/
Matrix *matrix_multiply(Matrix *, Matrix *);

/* Complexity: O(N*n+N*m);
Returns the rectangular submatrix defined by an upper left (start) point and a bottom right point (end).
For each non-zero element in the range, inserts an element in the submatrix.
In short, does 'N' inserts (each one with complexity O(n+m)) where 'N' is the number of non-zeros in the range.
*/
Matrix *matrix_slice(Matrix *, int, int, int, int);

/* Complexity: O(q*n+q*m);
Swaps (in place) the position between two lines of the matrix.
For that, uses a loop to go through each column changing the elements between the input lines.
Except for the best case, does '2q' insertions (each one with complexity O(n+m)), where 'q' is the number of columns of the matrix.
*/
void matrix_swapLines(Matrix *, int, int);

/* Complexity: O(q*n+q*m);
Swaps (in-place) the position between two columns of the matrix.
For that, uses a loop to go through each line changing the elements between the input columns.
Except for the best case, does '2q' insertions (each one with complexity O(n+m)), where 'q' is the number of lines of the matrix.
*/
void matrix_swapColumns(Matrix *, int, int);

#endif
