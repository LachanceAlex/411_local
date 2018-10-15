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
    assert(blocksize > 0);
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
    int blocksize = (int) floor(sqrt((double) (64 * 1024) / (double) size));
    if (blocksize == 0)
        blocksize = 1;
    // as big as possible
    assert((blocksize+1) * (blocksize+1) * size > 64 * 1024);
    if (size <= 64 * 1024)
        assert(blocksize * blocksize * size <= 64 * 1024); // no bigger
    return Array2b_new(width, height, size, blocksize);
}

void Array2b_free (T *array2b){
    assert(array2b && *array2b);
    T array = *array2b;
    free(array->blocks);
    FREE(*array2b);
}

int Array2b_width (T array2b){
    assert(array2b);
    return array_2b->width;
}

int Array2b_height(T array2b){
    assert(array2b);
    return array_2b->height;
}

int Array2b_size (T array2b){
    assert(array2b);
    return array_2b->size;
}

int Array2b_blocksize(T array2b){
    assert(array2b);
    return array_2b->blocksize;
}

void *Array2b_at(T array2b, int i, int j){
    assert(i >= 0 && j >= 0);
    int b  = array2b->blocksize;
    int loc = ((j / b)* ((array2b->width + b - 1) / b) + (i / b))*(b*b) + (i % b) * b + j % b;
    return &((unsigned char*)array2b->blocks)[loc * array2b->size];
}

void Array2b_map(T array2b,
  void apply(int i, int j, T array2b, void *elem, void *cl), void *cl){
    assert(array2b);
    int h = array2b->height;
    int w = array2b->width;
    int b = array2b->blocksize;
    void* blocks = array2b->blocks;
    int size = array2b->size;
    int bw = (w + b - 1) / b;
    int bh = (h + b - 1) / b;

    for (int bx = 0; bx < bw; bx++) {
        for (int by = 0; by < bh; by++) {
            for(int i0 = 0; i0 < b; i0++) {
                int bi = bx * b + i0;
                for(int j0 = 0; j0 < b; j0++) {
                    int bj = by * b + j0;
                    if(bi < w && bj < h){  
                        int loc = (by * bw + bx)*(b*b) + (bi % b) * b + bj % b;
                        apply(bi, bj, array2b, &((unsigned char*)blocks)[loc*size], cl);
                    }
                }
            }
        }
    }
}

#undef T
