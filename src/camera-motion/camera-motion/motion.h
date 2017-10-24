#ifndef __MOTION_H__
#define __MOTION_H__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <time.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
//#include <SDL/SDL.h>
#include "klt/utils.h"
#include "klt/pnmio.h"
#include "klt/klt.h"

/**********************************
* F0 - Pesos iniciais             *
* F1 - Ajuste dos pesos iniciais  *
* F2 - Ajuste iterativo dos pesos *
**********************************/

#define F0 1 
#define F1 1 
#define F2 1 

typedef struct _matrix {
   float **x;
   float **y;
} Matrix;

double* compute_camera_motion (
   KLT_TrackingContext tc,
   unsigned char *frame_prv,
   KLT_FeatureList feat_list_prv,
   unsigned char *frame_act,
   KLT_FeatureList feat_list_act,
   int width,
   int height,
   int g_rows, 
   int g_cols,
   int step,
   int iframe,
   FILE *descriptor
);

void      weights_redefinition  (Matrix *m, float **weights, int rows, int cols);

void      weights_correction    (float **weights, float **kappa, float **error, KLT_FeatureList fl, int rows, int cols);

void without_correction (float **weights, float **kappa, float **error, KLT_FeatureList fl, int rows, int cols);

void      copy_features         (KLT_FeatureList fl1, KLT_FeatureList fl2);

void      erase_features        (KLT_FeatureList fl1, KLT_FeatureList fl2);

void      grid_dimension        (int *g_rows, int *g_cols, int b_x, int b_y, int w_width, int nskip, int cols, 
                                 int rows, int step);
void      copy_matrix_model     (Matrix *source, Matrix *target, int w, int h); 

Matrix *  alloc_matrix_model    (int w, int h);

float **  alloc_mat_float       (int w, int h);

void      free_mat_float        (float **mat, int h);

Matrix *  klt_optical_flow      (KLT_FeatureList actual_fl, KLT_FeatureList previous_fl, float **weights, int rows, int cols);

void      clean_model           (Matrix *model, int rows, int cols);

Matrix *  pan_model             (int rows, int cols);

Matrix *  tilt_model            (int rows, int cols);

Matrix *  zoom_model            (int grows, int gcols, int irows, int icols);

Matrix *  roll_model            (int grows, int gcols, int irows, int icols);

void      solve_system          (double *eq_models, double *eq_sol, double *eq_flows, int size);

float     product_scale         (Matrix *model, Matrix *flow, float **pesos, int rows, int cols);

float     least_square          (Matrix *m1, Matrix *m2, float **weights, int rows, int cols);

Matrix *  rnxm_diff             (Matrix *m1, Matrix *m2, int rows, int cols );

Matrix *  rnxm_scale_mult       (Matrix *m, double scale, int rows, int cols );

Matrix *  rnxm_sum              (Matrix *m1, Matrix *m2, Matrix *m3, Matrix *m4, float **weights, int rows, int cols);

float     sum_weights           (float **weights, int cols, int rows);
	
void      fix_weights           (Matrix *m1, Matrix *m2, float **weights, int rows, int cols, int *loop);

char *    file_name             (char *file, char *information, char *type, int frame);

void      write_to_file         (FILE *f, int act_frame, int prv_frame, double *dmat_x, float residue, float s_wts);
 
void      SetCPUTime            (clock_t* value);

double    GetElapsedCPUTime     ();

void      disalloc_matrix_model (Matrix *m, int rows);

float     diagonal              (int rows, int cols);

void copy_mfloat (float **d, float **s, int rows, int cols);

void statiflow                  (float **rx, float **ry, float **w, int rows, int cols, float *mean_x, float *mean_y, float *std); 

void print_flows                (Matrix *m1, Matrix *m2, float **weights, float **kappa, float **stwj, int g_rows, int g_cols, int frame, int jump, char *name);

#endif
