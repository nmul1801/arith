/*******************************************************
 * HEADER
 ******************************************************/

#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

typedef struct UArray2b_T *UArray2b_T;

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <error.h>
#include <uarray.h>
#include <math.h>
#include <stdbool.h>

/*
* new blocked 2d array
* blocksize = square root of # of cells in block.
* blocksize < 1 is a checked runtime error
*/
extern UArray2b_T UArray2b_new (int width, int height, int size, int blocksize);
/* new blocked 2d array: blocksize as large as possible provided
* block occupies at most 64KB (if possible)
*/
extern UArray2b_T UArray2b_new_64K_block(int width, int height, int size);
extern void UArray2b_free (UArray2b_T *array2b);
extern int UArray2b_width (UArray2b_T array2b);
extern int UArray2b_height (UArray2b_T array2b);
extern int UArray2b_size (UArray2b_T array2b);
extern int UArray2b_blocksize(UArray2b_T array2b);
/* return a pointer to the cell in the given column and row.
* index out of range is a checked run-time error
*/
extern void *UArray2b_at(UArray2b_T array2b, int column, int row);
/* visits every cell in one block before moving to another block */
extern void UArray2b_map(UArray2b_T array2b,
void apply(int col, int row, UArray2b_T array2b,
void *elem, void *cl),
void *cl);
bool isValid(UArray2b_T array2b, int colNum, int rowNum);
void printArr(UArray2b_T array2b);
/*
* it is a checked run-time error to pass a NULL T
* to any function in this interface
*/
#undef UARRAY2B_INCLUDED
#endif