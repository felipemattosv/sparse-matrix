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

#endif
