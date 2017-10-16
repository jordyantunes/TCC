#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int imax (int a, int b);

int imin (int a, int b);

double dmax (double a, double b);

double dmin (double a, double b);

double** alloc_dmatrix (int ncols, int nrows);

void disalloc_dmatrix (double **matrix, int nrows);

float*** alloc_ndim_float_vect (int dim1, int dim2, int dim3);

#endif
