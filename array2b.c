#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array2b.h"
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"

#define T Array2b_T
struct T {
  int height;
  int width;
  int blocksize;
  int size;
  Array_T array;
};

/* new blocked 2d array: blocksize = square root of # of cells in block */
T Array2b_new(int width, int height, int size, int blocksize){

  T array_2b;
  NEW(array_2b);

  array_2b->width = width;
  array_2b->height = height;
  array_2b->size = size;
  array_2b->blocksize = blocksize;

  Array2b_new_64K_block();

  return array_2b;

}
/* new blocked 2d array: blocksize as large as possible provided
block occupies at most 64KB (if possible) */
T Array2b_new_64K_block(int width, int height, int size){

}

void Array2b_free (T *array2b){
  FREE(array_2b);
}

int Array2b_width (T array2b){
  return array_2b->width;
}

int Array2b_height(T array2b){
  return array_2b->height;
}

int Array2b_size (T array2b){
  return array_2b->size;
}

int Array2b_blocksize(T array2b){
  return array_2b->blocksize;
}

void *Array2b_at(T array2b, int i, int j){

}

void Array2b_map(T array2b,
  void apply(int i, int j, T array2b, void *elem, void *cl), void *cl){

}

#undef T
