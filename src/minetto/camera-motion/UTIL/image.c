#include "image.h"

/* */
void get_image_dimensions (char *filename, int *nrows, int *ncols) {

   FILE *file = fopen (filename, "r");

   char *type = (char *)malloc(3*sizeof(char));

   fscanf (file, "%s", type);

   free(type);

   fscanf (file, "%d %d", ncols, nrows);

   fclose (file);
}   

/* */
unsigned char* read_pgm (char *filename, int *nrows, int *ncols) {

   FILE *file = fopen (filename, "r");

   char *type = (char *)malloc(3*sizeof(char));

   fscanf (file, "%s", type);

   free(type);

   fscanf (file, "%d %d", ncols, nrows);

   int size = (*nrows) * (*ncols);

   unsigned char *img = (unsigned char *)malloc(size * sizeof(char));

   int i;
   for (i = 0; i < size; i++) {
      int pixel;
      fscanf (file, "%d", &pixel); 
      img[i] = (unsigned char)pixel;
   }
   fclose (file);
   return img;
}

/* */
void write_uchar_to_pgm (char *filename, unsigned char *img, int nrows, int ncols) {

   FILE *file = fopen (filename, "w");

   fprintf (file, "P2\n");

   fprintf (file, "%d %d\n", ncols, nrows);

   fprintf (file, "255\n");

   int size = nrows * ncols;

   int i;
   for (i = 0; i < size; i++) {
      fprintf (file, "%d ", img[i]); 
      if (i % 12 == 0) { fprintf (file, "\n"); }
   }
   fclose (file);
}

/* */
void write_uchar_to_ppm (char *filename, unsigned char *img, int nrows, int ncols) {

   FILE *file = fopen (filename, "w");

   fprintf (file, "P3\n");

   fprintf (file, "%d %d\n", ncols, nrows);

   fprintf (file, "255\n");

   int i, j;
   for (i = 0; i < nrows; i++) {
      for (j = 0; j < ncols; j++) {
         int r = img[3 * i * ncols + 3 * j + 0]; /*red*/
         int g = img[3 * i * ncols + 3 * j + 1]; /*green*/
         int b = img[3 * i * ncols + 3 * j + 2]; /*blue*/
         //int a = img[4 * i * ncols + 4 * j + 3]; /*blue*/
         fprintf (file, "%d %d %d ", r, g, b); 
         if (j % 12 == 0) { fprintf (file, "\n"); }
      }
   }
   fclose (file);
}

/* */
void write_argb_to_ppm (char *filename, unsigned char *img, int nrows, int ncols) {

   FILE *file = fopen (filename, "w");

   fprintf (file, "P3\n");

   fprintf (file, "%d %d\n", ncols, nrows);

   fprintf (file, "255\n");

   int i, j;
   for (i = 0; i < nrows; i++) {
      for (j = 0; j < ncols; j++) {
         int r = img[4 * i * ncols + 4 * j + 0]; /*red*/
         int g = img[4 * i * ncols + 4 * j + 1]; /*green*/
         int b = img[4 * i * ncols + 4 * j + 2]; /*blue*/
         int a = img[4 * i * ncols + 4 * j + 3]; /*blue*/
         a = 0;
         fprintf (file, "%d %d %d ", r, g, b); 
         if (j % 12 == 0) { fprintf (file, "\n"); }
      }
   }
   fclose (file);
}


void image_change_interval (double *src, int nrows, int ncols, double vmax, double vmin, int min, int max, unsigned char *dst) {

   int i;
   int size = nrows * ncols;

   if (vmax == vmin) {
      vmax = -HUGE_VAL; vmin = +HUGE_VAL;
      for (i = 0; i < size; i++) {
         vmax = fmax (vmax, src[i]);
         vmin = fmin (vmin, src[i]);
      }
   }
     
   for (i = 0; i < size; i++) {
      dst[i] = ( (max - min)*(src[i] - vmin) )/(vmax - vmin) + min;
   } 
}

/* */
void write_int_to_pgm (char *filename, int *img, int nrows, int ncols) {

   FILE *file = fopen (filename, "w");

   fprintf (file, "P2\n");

   fprintf (file, "%d %d\n", ncols, nrows);

   fprintf (file, "255\n");

   int size = nrows * ncols;

   int i;
   for (i = 0; i < size; i++) {
      fprintf (file, "%d ", img[i]); 
      if (i % 12 == 0) { fprintf (file, "\n"); }
   }
   fclose (file);
}

/* */
void write_double_to_pgm (char *filename, double *img, int nrows, int ncols) {

   FILE *file = fopen (filename, "w");

   fprintf (file, "P2\n");

   fprintf (file, "%d %d\n", ncols, nrows);

   fprintf (file, "255\n");

   int size = nrows * ncols;

   int i;
   for (i = 0; i < size; i++) {
      fprintf (file, "%d ", (int)img[i]); 
      if (i % 12 == 0) { fprintf (file, "\n"); }
   }
   fclose (file);
}

/* */
unsigned char* convert_argb_to_gray (unsigned char *image, int nrows, int ncols) {

   int x, y;

   int size = nrows * ncols;

   unsigned char *out = (unsigned char *)malloc(size * sizeof(unsigned char));

   for (y = 0; y < nrows; y++) {
      for (x = 0; x < ncols; x++) {
         int r = image[4 * y * ncols + 4 * x + 0]; /*red*/
         int g = image[4 * y * ncols + 4 * x + 1]; /*green*/
         int b = image[4 * y * ncols + 4 * x + 2]; /*blue*/
         int a = image[4 * y * ncols + 4 * x + 3]; /*alpha*/
         //r /= 255.0; g /= 255.0; b /= 255.0;
         unsigned char gray = (unsigned char)(0.299*r + 0.587*g + 0.114*b);
         out[y * ncols + x] = gray;
         a = 0; /*The alpha channel is not used.*/
      }
   }
   return out;
}

/* */
void draw_rectangle (unsigned char *image, int nrows, int ncols, int x, int y, int w, int h, int color) {
   int k;
   for (k = x; k < (x + w); k++) {
      image[y * ncols + k] = color;
      image[(y + h)* ncols + k] = color;
   }
   for (k = y; k < (y + h); k++) {
      image[k * ncols + x] = color;
      image[k * ncols + (x + w)] = color;
   }
}

/* */
unsigned char* convert_rgb_to_gray (unsigned char *image, int nrows, int ncols) {

   int x, y;

   int size = nrows * ncols;

   unsigned char *out = (unsigned char *)malloc(size * sizeof(unsigned char));

   for (y = 0; y < nrows; y++) {
      for (x = 0; x < ncols; x++) {
         int r = image[3 * y * ncols + 3 * x + 0]; /*red*/
         int g = image[3 * y * ncols + 3 * x + 1]; /*green*/
         int b = image[3 * y * ncols + 3 * x + 2]; /*blue*/
         //r /= 255.0; g /= 255.0; b /= 255.0;
         unsigned char gray = (unsigned char)(0.299*r + 0.587*g + 0.114*b);
         out[y * ncols + x] = gray;
      }
   }
   return out;
}

/* */
unsigned char* copy_uchar (unsigned char *src, int nrows, int ncols) {
   
   int size = nrows * ncols;

   unsigned char *dst = (unsigned char *)malloc(size * sizeof(unsigned char));

   int i;
   for (i = 0; i < size; i++) {
      dst[i] = src[i]; 
   }
   return dst;
}

/*Assumes that both images {src} and {dst} have the same size {nrows*cols}. */
void copy_uchar_to_double (unsigned char *src, int nrows, int ncols, double *dst) {

   int size = nrows * ncols;

   int i;
   for (i = 0; i < size; i++) {
      dst[i] = src[i];
   }
}

/* */
void fill_double_image (double *frame, int nrows, int ncols, double val) {

   int size = nrows * ncols;

   int i;
   for (i = 0; i < size; i++) {
      frame[i] = val;
   }
}

/* */
void invert_image (int xmin, int ymin, int xmax, int ymax, unsigned char *src, unsigned char *dst, int nrows, int ncols) {
   int x, y;
   for (y = ymin; y < ymax; y++) {
      for (x = xmin; x < xmax; x++) {
         int i = y * ncols + x;
         dst[i] = 255 - src[i];
      }
   }   
}

unsigned char *resize_gray_uchar_image_bilinear (unsigned char *pixels, int w, int h, int w2, int h2) {

    unsigned char *temp = (unsigned char *)malloc(w2*h2*sizeof(unsigned char));
    int A, B, C, D, x, y, index, gray ;
    float x_ratio = ((float)(w-1))/w2 ;
    float y_ratio = ((float)(h-1))/h2 ;
    float x_diff, y_diff;
    int offset = 0 ;
    int i, j;
    for (i=0;i<h2;i++) {
        for (j=0;j<w2;j++) {
            x = (int)(x_ratio * j) ;
            y = (int)(y_ratio * i) ;
            x_diff = (x_ratio * j) - x ;
            y_diff = (y_ratio * i) - y ;
            index = y*w+x ;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = pixels[index] & 0xff ;
            B = pixels[index+1] & 0xff ;
            C = pixels[index+w] & 0xff ;
            D = pixels[index+w+1] & 0xff ;
            
            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            gray = (int)(
                    A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                    C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)
                    ) ;

            temp[offset++] = gray ;                                   
        }
    }
    return temp ;
}

double *resize_gray_uchar_double_image_bilinear (unsigned char *pixels, int w, int h, int h2, int w_mod, int *W2) {

    double y_ratio = (double)(h2)/(double)(h);
    int w2 = (int)(floor(w*y_ratio/w_mod + 0.5))*w_mod;
    *W2 = w2; 
    assert((w2 % w_mod) == 0);
    double x_ratio = (double)(w2)/(double)(w); 

    double *temp = (double *)malloc(w2*h2*sizeof(double));
    int A, B, C, D, x, y, index, gray ;
    double x_diff, y_diff;
    int offset = 0 ;
    int i, j;
    for (i=0;i<h2;i++) {
        for (j=0;j<w2;j++) {
            x = (int)(x_ratio * j) ;
            y = (int)(y_ratio * i) ;
            x_diff = (x_ratio * j) - x ;
            y_diff = (y_ratio * i) - y ;
            index = y*w+x ;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = pixels[index] & 0xff ;
            B = pixels[index+1] & 0xff ;
            C = pixels[index+w] & 0xff ;
            D = pixels[index+w+1] & 0xff ;
            
            gray =  ( A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) + C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff) );

            temp[offset++] = gray ;                                   
        }
    }
    return temp ;
}

/**/
unsigned char *resize_color_uchar_image_bilinear (unsigned char *pixels, int w, int h, int w2, int h2, int alpha) {

    int nchannels;

    if (alpha) { nchannels = 4; }

    else { nchannels = 3; }

    int nch = 3;
    unsigned char *temp = (unsigned char *)malloc(nch*w2*h2*sizeof(unsigned char));
    int A, B, C, D, x, y, index, gray ;
    float x_ratio = ((float)(w-1))/w2 ;
    float y_ratio = ((float)(h-1))/h2 ;
    float x_diff, y_diff;
    int offset = 0 ;
    int i, j;
    //printf("lala\n");
    for (i=0;i<h2;i++) {
        for (j=0;j<w2;j++) {
            x = (int)(x_ratio * j);
            y = (int)(y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            int c;
            for (c = 0; c < nch; c++) {
               index = nchannels * y * w + nchannels * x + c;
               //if (index < nchannels*w*h) {
                  //printf("aqui 00\n");
                  A = pixels[nchannels * y * w + nchannels * x + c];
               //}   
               //if (index+1 < nchannels*w*h) {
                  //printf("aqui 01\n");
                  B = pixels[nchannels * y * w + nchannels * (x+1) + c];
               //}   
               //if (index+w < nchannels*w*h) {
                  //printf("aqui 02\n");
                  C = pixels[nchannels * (y+1) * w + nchannels * x + c];
               //}   
               //if (index+w+1 < nchannels*w*h) {
                 // printf("aqui 03\n");
                  D = pixels[nchannels * (y+1) * w + nchannels * (x+1) + c];
               //}  
               //printf("aqui 5\n");
               temp[offset++] = ( A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) + C*(y_diff)*(1-x_diff) + D*(x_diff*y_diff));
               //printf("aqui 6\n");
            }
        }
    }
    //printf("aqui 12\n");
    return temp ;
}



