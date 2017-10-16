#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "klt/utils.h"
#include "klt/pnmio.h"
#include "klt/klt.h"
#include "motion.h"
#include "./PNG/png.h"
#include "./UTIL/image.h"

void initialize_descritor_matrix (float **mdesc, int cols, int rows) {
   int c;
   for (c = 0; c < cols; c++) {
      mdesc[0][c] = mdesc[1][c] = mdesc[2][c] = mdesc[3][c] = mdesc[4][c] = 0;
   }
}


int main (int argc, char *argv[]) {

   unsigned int nrows, ncols;
   unsigned char *colorpng1, *colorpng2;

   unsigned error1 = lodepng_decode32_file (&colorpng1, &ncols, &nrows, argv[1]);
   if(error1) printf("error %u: %s\n", error1, lodepng_error_text(error1));
   unsigned char *frame_act = convert_argb_to_gray (colorpng1, nrows, ncols);

   unsigned error2 = lodepng_decode32_file (&colorpng2, &ncols, &nrows, argv[2]);
   if(error2) printf("error %u: %s\n", error2, lodepng_error_text(error2));
   unsigned char *frame_prv = convert_argb_to_gray (colorpng2, nrows, ncols);
  
   FILE *descriptor = fopen(argv[3], "w");

   int nFeatures;

   /*Tracking context*/
   KLT_TrackingContext tc = KLTCreateTrackingContext();

   /*Number of cols and rows of the grid image*/
   int g_rows, g_cols;

   //unsigned char *frame_act = NULL;

   //unsigned char *frame_prv = NULL;
      
   int iframe = 1;

   int step = 16;

   grid_dimension (&g_rows, &g_cols, tc->borderx, tc->bordery, tc->window_width, tc->nSkippedPixels, ncols, nrows, step);
 
   nFeatures = g_rows * g_cols;

   printf("nrows: %d, ncols: %d, Grid nrows: %d, ncols: %d\n", nrows, ncols, g_rows, g_cols); 

   /*Feature lists*/
   KLT_FeatureList feat_list_act = KLTCreateFeatureList (nFeatures);

   KLT_FeatureList feat_list_prv = KLTCreateFeatureList (nFeatures);

   int columns = 200; 
 
   float **mdesc = alloc_mat_float (columns, 5);

   initialize_descritor_matrix (mdesc, columns, 5); 
 
   double *desc = compute_camera_motion (tc, frame_prv, feat_list_prv, frame_act, feat_list_act, ncols, nrows, g_rows, g_cols, step, iframe, descriptor);

   free(desc); 

   return 0;
}
