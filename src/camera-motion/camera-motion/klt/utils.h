#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <math.h>
#include <malloc.h>

void rmxn_inv (int n, double *A, double *R);

void rmxn_map_col (int m, int n, double *A, double *x, double *r);

void gsel_triangularize (int m, int n, double M[], int total);

void gsel_diagonalize(int m, int n, double M[], int total);

void gsel_normalize(int m, int n, double M[], int total);

#endif

