#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define INTMAX +9999999
#define INTMIN -9999999

void get_image_dimensions (char *filename, int *nrows, int *ncols);

unsigned char* read_pgm (char *filename, int *nrows, int *ncols);

void write_uchar_to_pgm (char *filename, unsigned char *img, int nrows, int ncols);

void write_uchar_to_ppm (char *filename, unsigned char *img, int nrows, int ncols);

void write_argb_to_ppm (char *filename, unsigned char *img, int nrows, int ncols);

unsigned char* convert_argb_to_gray (unsigned char *image, int nrows, int ncols);

unsigned char* convert_rgb_to_gray (unsigned char *image, int nrows, int ncols);

void write_double_to_pgm (char *filename, double *img, int nrows, int ncols);

void write_int_to_pgm (char *filename, int *img, int nrows, int ncols);

unsigned char* copy_uchar (unsigned char *src, int nrows, int ncols);

void draw_rectangle (unsigned char *image, int nrows, int ncols, int x, int y, int w, int h, int color);

void copy_uchar_to_double (unsigned char *src, int nrows, int ncols, double *dst);

void fill_double_image (double *frame, int nrows, int ncols, double val);

void invert_image (int xmin, int ymin, int xmax, int ymax, unsigned char *src, unsigned char *dst, int nrows, int ncols);

unsigned char *resize_gray_uchar_image_bilinear (unsigned char *pixels, int w, int h, int w2, int h2);

void image_change_interval (double *src, int nrows, int ncols, double vmax, double vmin, int min, int max, unsigned char *dst);

double *resize_gray_uchar_double_image_bilinear (unsigned char *pixels, int w, int h, int h2, int w_mod, int *W2);

unsigned char *resize_color_uchar_image_bilinear (unsigned char *pixels, int w, int h, int w2, int h2, int alpha);

#endif

