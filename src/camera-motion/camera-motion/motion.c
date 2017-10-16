/* 
** Written by Rodrigo Minetto 
**
** Problem: Compute the camera motion between two pairs of frames.
** The movements calculated are: pan, zoom, tilt and roll.
**
** Copyright notice:
**
** Permission to use this software for any purpose is hereby granted,
** provided that any substantial copy or mechanically derived version
** of this file that is made available to other parties is accompanied
** by this copyright notice in full, and is distributed under these same
** terms. 
**
** DISCLAIMER: This software is provided "as is" with no explicit or
** implicit warranty of any kind.  Neither the authors nor their
** employers can be held responsible for any losses or damages
** that might be attributed to its use.
**
** End of copyright notice.
*/

#include "motion.h"

clock_t CPUstart, CPUend;

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
) { 

   int iterations = 0;

   /*Controls the elimination of wrong direction vectors*/
   int loop;
 
   /*Number of models (pan, tilt, zoom)*/
   int n_models = 4; 
 
   /*Vectors to compute the camera movement*/
   double *eq_sol = (double *)malloc((n_models + 1)*sizeof(double));
   double eq_models[n_models*n_models], eq_flows[n_models];
 
   /*Matrices to store the weights and error of each position: */
   float **weights = alloc_mat_float (g_cols, g_rows);
   float **kappa = alloc_mat_float (g_cols, g_rows);
   float **stwj = alloc_mat_float (g_cols, g_rows);
   float **error = alloc_mat_float (g_cols, g_rows);

   /*Canonical flows definition*/
   Matrix *pan  = pan_model  (g_rows, g_cols);
   Matrix *tilt = tilt_model (g_rows, g_cols);
   Matrix *zoom = zoom_model (g_rows, g_cols, height, width);
   Matrix *roll = roll_model (g_rows, g_cols, height, width);

   /*Models*/
   //Matrix *pan_tmp, *pan, *tilt_tmp, *tilt, *zoom_tmp, *zoom, *roll_tmp, *roll, *flow, *fitt;

   Matrix *tmp = alloc_matrix_model (g_cols, g_rows); 

   char filename[256];

   KLTSelectGoodFeatures (tc, frame_prv, width, height, feat_list_act, weights, step);  

   //sprintf(filename, "./saida/select_%06d.ppm", iframe);
   //KLTWriteFeatureListToPPM (feat_list_act, frame_prv, width, height, filename);

   copy_features (feat_list_act, feat_list_prv);

   KLTTrackFeatures (tc, frame_prv, frame_act, width, height, feat_list_act, error, g_cols);

   //sprintf(filename, "./saida/track_%06d.ppm", iframe);
   //KLTWriteFeatureListToPPM (feat_list_act, frame_act, width, height, filename);

   /*Adjust the weights according with the match of the KLT method*/

   if (F0 == 1) {
      weights_correction (weights, kappa, error, feat_list_act, g_rows, g_cols);
      printf("Com pesos iniciais\n");
   }
   else {
      without_correction (weights, kappa, error, feat_list_act, g_rows, g_cols); 
   }

   /*Velocity of the points*/
   Matrix *flow = klt_optical_flow (feat_list_act, feat_list_prv, weights, g_rows, g_cols);

   /*Give more weight to stopped features*/
   if (F1 == 1) {
      weights_redefinition (flow, weights, g_rows, g_cols);
      printf("Com ajuste dos pesos iniciais\n");
   }

   copy_mfloat (stwj, weights, g_rows, g_cols);

   float residue = 0.0;

   do {
      /*Calculates the better P, T, Z and R, see article[?]*/
      
      /*Pan row*/
           eq_models[0]  = product_scale  (pan,  pan,  weights, g_rows, g_cols);
           eq_models[1]  = product_scale  (pan,  tilt, weights, g_rows, g_cols);
           eq_models[2]  = product_scale  (pan,  zoom, weights, g_rows, g_cols);
           eq_models[3]  = product_scale  (pan,  roll, weights, g_rows, g_cols);
           
           /*Tilt row*/
           eq_models[4]  = product_scale  (tilt, pan,  weights, g_rows, g_cols);
           eq_models[5]  = product_scale  (tilt, tilt, weights, g_rows, g_cols);
           eq_models[6]  = product_scale  (tilt, zoom, weights, g_rows, g_cols);
           eq_models[7]  = product_scale  (tilt, roll, weights, g_rows, g_cols);
            
           /*Zoom row*/
           eq_models[8]  = product_scale  (zoom, pan,  weights, g_rows, g_cols);
           eq_models[9]  = product_scale  (zoom, tilt, weights, g_rows, g_cols);
           eq_models[10] = product_scale  (zoom, zoom, weights, g_rows, g_cols);
           eq_models[11] = product_scale  (zoom, roll, weights, g_rows, g_cols);
           
           /*Roll row*/
           eq_models[12] = product_scale  (roll, pan,  weights, g_rows, g_cols);
           eq_models[13] = product_scale  (roll, tilt, weights, g_rows, g_cols);
           eq_models[14] = product_scale  (roll, zoom, weights, g_rows, g_cols);
           eq_models[15] = product_scale  (roll, roll, weights, g_rows, g_cols);
           
           /*Flow row*/
           eq_flows[0]   = product_scale  (pan,  flow, weights, g_rows, g_cols);
           eq_flows[1]   = product_scale  (tilt, flow, weights, g_rows, g_cols);
           eq_flows[2]   = product_scale  (zoom, flow, weights, g_rows, g_cols);
           eq_flows[3]   = product_scale  (roll, flow, weights, g_rows, g_cols);
           
           /*Solve the linear system to find the most reliable camera parameters*/
           solve_system (eq_models, eq_sol, eq_flows, n_models);

           /*Combination of the canonical flows*/
           Matrix *pan_tmp  = rnxm_scale_mult (pan,  eq_sol[0], g_rows, g_cols);
           Matrix *tilt_tmp = rnxm_scale_mult (tilt, eq_sol[1], g_rows, g_cols);
           Matrix *zoom_tmp = rnxm_scale_mult (zoom, eq_sol[2], g_rows, g_cols);
           Matrix *roll_tmp = rnxm_scale_mult (roll, eq_sol[3], g_rows, g_cols);

           Matrix *fitt = rnxm_sum (pan_tmp, tilt_tmp, zoom_tmp, roll_tmp, weights, g_rows, g_cols);

           residue = least_square (flow, fitt, weights, g_rows, g_cols);

           loop = 1;
          
           //print_klt_flow (flow, weights, g_rows, g_cols, iterations);
 
           /*Eliminates out-liers of the fitted flow*/
           if (F2 == 1) {
               fix_weights (flow, fitt, weights, g_rows, g_cols, &loop);
           } 
           //contador++;

           //if ((floor(iframe / start_position) == 1) && (!print_data)) 
           //    copy_matrix_model (fitt, tmp, g_rows, g_cols);

           disalloc_matrix_model (pan_tmp,  g_rows);             
           disalloc_matrix_model (tilt_tmp, g_rows);             
           disalloc_matrix_model (zoom_tmp, g_rows);             
           disalloc_matrix_model (roll_tmp, g_rows);             
           disalloc_matrix_model (fitt,  g_rows);            
           iterations++; 
 
   } while (!loop);

   /*store the sum of all the weights of the grid*/
   float sum_w = sum_weights (weights, g_cols, g_rows);

   /*values normalization*/
   int jump = 1, frame_rate = 1;
   eq_sol[0] = (eq_sol[0] * ((float)jump / (float)frame_rate) ) / ((float) g_cols);
   eq_sol[1] = (eq_sol[1] * ((float)jump / (float)frame_rate) ) / ((float) g_cols);
   eq_sol[2] = (eq_sol[2] * ((float)jump / (float)frame_rate) ) / ((float) g_cols);
   eq_sol[3] = (eq_sol[3] * ((float)jump / (float)frame_rate) ) / ((float) g_cols); 
   eq_sol[4] = sqrtf(residue/sum_w);

   /*MUDAR O NULL para algum arquivo valido*/
   write_to_file (descriptor, iframe-1, iframe, eq_sol, residue, sum_w);

   /*Writing in screen: */
   /*int pan_shift = 100;
   int x_shift = 200;
   int scale = 100;

   int x, y;
   for (x = x_shift; x >= 1; x--) {

       pFrame->data[0][((int)(eq_sol[0]*scale) + pan_shift) * pFrame->linesize[0] + (x-1)] = pFrame->data[0][((int)(eq_sol[0]*pan_shift) + y_shift) * pFrame->linesize[0] + x];

   }
   pFrame->data[0][((int)(eq_sol[0]*pan_shift) + y_shift) * pFrame->linesize[0] + x] = 255; */

   //KLTWriteFeatureListToPPM(fl1, img1, cols, rows, "feat2.ppm");

   disalloc_matrix_model (pan, g_rows);
   disalloc_matrix_model (tilt, g_rows);
   disalloc_matrix_model (zoom, g_rows);
   disalloc_matrix_model (roll, g_rows);
   disalloc_matrix_model (tmp, g_rows);
   erase_features (feat_list_prv, feat_list_act);
   return eq_sol;
}

/*******************************************************
 * Function: sum the weights of all the positions.     *
 * Inputs:                                             *
 * weights: the matrix weights.                        *
 * rows, cols : dimensions of the grid.                *
 * Output:                                             *
 * sum : sum of the weights.                           *
 * ****************************************************/
float sum_weights (float **weights, int cols, int rows)
{
    int   i, j;
    float sum = 0.0;

    for (i = 0; i < rows; i++) {  
        for (j = 0; j < cols; j++) { 
            sum += weights[i][j];
        }
    }
    return sum;
}

/*******************************************************
 * Function: given two tracked windows in two different *
 * images and the error adjust the weight of position. *
 * Inputs:                                             *
 * weights: the matrix weights.                        *
 * error: klt adjustment error.                        *
 * fl: feature list.                                   *
 * rows, cols : dimensions of the grid.                *
 * ****************************************************/
void weights_correction (float **weights, float **kappa, float **error, KLT_FeatureList fl, int rows, int cols)
{
    int i, j, k;

    for (i = 0, k = 0; i < rows; i++) {
        for (j = 0; j < cols; j++, k++) {
            if (fl->feature[k]->val == KLT_TRACKED) {
                if (error[i][j] > 1) {
                    weights[i][j] = weights[i][j] / error[i][j];
                }
            }
            else {
                weights[i][j] = 0.0;
            }
            kappa[i][j] = weights[i][j];
        }
    }
}

void without_correction (float **weights, float **kappa, float **error, KLT_FeatureList fl, int rows, int cols)
{
    int i, j, k;

    for (i = 0, k = 0; i < rows; i++) {
        for (j = 0; j < cols; j++, k++) {
            if (fl->feature[k]->val == KLT_TRACKED) {
                weights[i][j] = 1.0;
            }
            else {
                weights[i][j] = 0.0;
            }           
            kappa[i][j] = weights[i][j];
        }
    }
}

/*******************************************************
 * Function: give more weight to stopped pixels.       *
 * Inputs:                                             *
 * m: KLT optical flow.                                *
 * weights: the matrix weights, gived by:              *
 *                                     min (gx^2  gxgy *
 *                                          gxgy  gy^2)*
 * rows, cols : dimensions of the grid.                *
 * ****************************************************/
void weights_redefinition (Matrix *m, float **weights, int rows, int cols)
{
    int i, j;
    float aux;
    float epsilon = 0.01;
    float norm = 0.0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            norm = sqrtf((m->x[i][j]*m->x[i][j]) + (m->y[i][j]*m->y[i][j])) + epsilon;
            if (norm != 0)
                aux = weights[i][j] / norm;
            else
                aux = 0;
            weights[i][j] = aux;
        }
    }
}

/*******************************************************
 * Function: eliminated vectors that are believed to be*
 * wrong, probably because aperture problems .         *
 * Inputs:                                             *
 * m1 and m2: the real (KLT) and expected movement.    *
 * weights: the matrix weights.                        *
 * rows, cols : dimensions of the grid.                *
 * loop : if at least one vector is erased then continues. *
 * ****************************************************/

void statiflow (float **rx, float **ry, float **w, int rows, int cols, float *mean_x, float *mean_y, float *std) 
{
    int i, j;
    float sum_wx = 0.0, sum_wy = 0.0, sum_w = 0.0;
    float sum_wd2 = 0.0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            sum_wx += w[i][j]*rx[i][j];
            sum_wy += w[i][j]*ry[i][j];
            sum_w  += w[i][j];
        }
    }

    if (sum_w != 0) {
        *mean_x = sum_wx / sum_w;
        *mean_y = sum_wy / sum_w;
    }
    else {
        *mean_x = 0.0;
        *mean_y = 0.0;
    }    

    /*Standard Deviation*/
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
                float dx = rx[i][j] - *mean_x; 
                float dy = ry[i][j] - *mean_y;
                float d2 = dx*dx + dy*dy; 
                sum_wd2 += w[i][j] * d2;
        }
    }
    if (sum_w != 0) {
        *std = sqrtf (sum_wd2 / sum_w);
    }
    else {
        *std = 0.0;
    }
}



void fix_weights (Matrix *m1, Matrix *m2, float **weights, int rows, int cols, int *loop)
{
    int   i, j;
    float mean_x;
    float mean_y;
    float std_dev;

    float **rx, **ry;

    rx = alloc_mat_float (cols, rows);
    ry = alloc_mat_float (cols, rows);

    /*Vetores de resíduo*/
    int stop = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            rx[i][j] = m2->x[i][j] - m1->x[i][j];
            ry[i][j] = m2->y[i][j] - m1->y[i][j];
            if (weights[i][j] > 500) { weights[i][j] = 500; }
            //printf("%f %f\n", rx[i][j], ry[i][j]);
            /*if ( (fabs(rx[i][j]) < 0.01) && (fabs(ry[i][j]) < 0.01) ) {
               stop++;
            }*/
        }
    }

    //printf("####################\n");
    //printf("Stop: %d\n", stop);

    /*...*/
    /*double percentage = ((double )stop) / ((double) (rows * cols));
    if (percentage < 0.4) {
      printf("####################\n");
      printf("Stop: %d\n", stop);
      for (i = 0; i < rows; i++) {
         for (j = 0; j < cols; j++) {
            if ( (fabs(rx[i][j]) < 0.01) && (fabs(ry[i][j]) < 0.01) ) {
               weights[i][j] = 0;
            }
            printf("%d %d - %f %f - w = %f\n", i, j, rx[i][j], ry[i][j], weights[i][j]);
          }
       }
    }*/

    statiflow (rx, ry, weights, rows, cols, &mean_x, &mean_y, &std_dev); 


    /*Erase Weights*/
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {

            float d = hypot (rx[i][j] - mean_x, ry[i][j] - mean_y);

            if ((d > (3 * std_dev)) && (weights[i][j] != 0)) { 
            //if ((d > (1 * std_dev)) && (weights[i][j] != 0)) { 
                weights[i][j] = *loop = 0;
            }
        }
    }
}

/*******************************************************************************
 * Function: solve a size x size system.                                       *
 * Inputs:                                                                     *
 * eq_models : a size x size matrix, with the models movements.                *
 * eq_sol : the undefined variables.                                           *
 * eq_flows : the flow movements values.                                       *
 * Obs: eq_models*eq_flows = eq_sol, where the vector eq_sol is to be solved.  *
 * ****************************************************************************/
void solve_system (double *eq_models, double *eq_sol, double *eq_flows, int size)
{
    double *tmp;

    tmp = (double *)malloc((size*size) * sizeof(double));
  
    rmxn_inv (size, eq_models, tmp);

    rmxn_map_col (size, size, tmp, eq_flows, eq_sol);
}


/******************************************************************************************
 * Function: creates a grid based on size of the input image  where on each position      *
 * will be applied the KLT feature tracking.                                              *
 * Inputs :                                                                               *
 * g_rows and g_cols: number of points on the grid at vertical and horizontal direction.  *
 * bx and by: displacement of the image border.                                           *
 * w_width: window width. If the variable step is zero, the displacement is equal w_width.*
 * skip: a klt parameter almost like step.                                                *
 * row and col: number of rows and cols of the input image.                               *
 * step: define the distance between each feature of the grid.                            *
 * ***************************************************************************************/
void grid_dimension (int *g_rows, int *g_cols, int b_x, int b_y, int w_width, int skip, int cols, int rows, int step)
{
   int x, y;
   *g_rows = 0;
   *g_cols = 0;
   int displace = w_width + 5;

   if (step > 0)
       w_width = step;

   for (y = b_y + displace; y < (rows - b_y) - displace; y += (skip + w_width))  {
       *g_rows += 1;
   }

   for (x = b_x + displace; x < (cols - b_x) - displace; x += (skip + w_width))  {
       *g_cols += 1;
   }
}

/*********************************************************************
 * Function: copy a list feature (target) to another (destination).  *
 * ******************************************************************/
void copy_features (KLT_FeatureList target, KLT_FeatureList destination) 
{
   int i;

   for (i = 0; i < target->nFeatures; i++)  {
      destination->feature[i]->x = target->feature[i]->x;
      destination->feature[i]->y = target->feature[i]->y;
   }
}

/***************************************************
 * Function: erase the features lists (f1, f2).    *
 * ************************************************/
void erase_features (KLT_FeatureList fl1, KLT_FeatureList fl2) 
{
    int i;

    for (i = 0; i < fl1->nFeatures; i++)  {
        fl1->feature[i]->x = fl1->feature[i]->y = fl2->feature[i]->x = fl2->feature[i]->y = 0;
    }
}

/*****************************************************
 * generate a string name based on:                  *
 * file : prefix of the input images.                *
 * information : information about the output image. *
 * type : pgm, ppm, eps.                             *
 * frame : frame of the sequence.                    *
 * **************************************************/
char * file_name (char *file, char *information, char *type, int frame)
{
    char *tmp;
    char *aux;

    tmp = (char *)malloc(255 * sizeof(char));
    aux = (char *)malloc(255 * sizeof(char));
   
    sprintf (aux, "%06d", frame);
    strcpy  (tmp, file);
    strcat  (tmp, information);
    strcat  (tmp, aux);
    strcat  (tmp, type);

    free(aux);    
    return tmp;
}

void copy_mfloat (float **d, float **s, int rows, int cols)
{  
    int i, j;

    for (i = 0; i < rows; i++) {
       for (j = 0; j < cols; j++) {
           d[i][j] = s[i][j]; 
           d[i][j] = s[i][j];
       }
    }
}

/***************************************************
 * Function: copy a matrix(x,y) of floats:         *
 * w : width or cols, h : height or rows           *
 * ************************************************/
void copy_matrix_model (Matrix *source, Matrix *target, int rows, int cols)
{  
    int i, j;

    for (i = 0; i < rows; i++) {
       for (j = 0; j < cols; j++) {
           target->x[i][j] = source->x[i][j]; 
           target->y[i][j] = source->y[i][j];
       }
    }
}

/***************************************************
 * Function: allocate a matrix(x,y) of floats:     *
 * w : width or cols, h : height or rows           *
 * ************************************************/
Matrix * alloc_matrix_model (int w, int h)
{  
    Matrix *m = (Matrix *) malloc (1 * sizeof(Matrix));;

    m->x = alloc_mat_float ( w, h );
    m->y = alloc_mat_float ( w, h );

    return m;
}

/***************************************************
 * Function: disallocate a matrix(x,y) of floats:  *
 * ************************************************/
void disalloc_matrix_model (Matrix *m, int rows)
{  
    free_mat_float ( m->x, rows );
    free_mat_float ( m->y, rows );
}

/*************************************************************************
 * Function: compute and scalar product between two models m1 and m2.    *
 * Out parameters: the scalar value resultant.                           *
 ************************************************************************/
float product_scale (Matrix *m1, Matrix *m2, float **weights, int rows, int cols)
{
   int i, j;
   float sum  = 0.0;
   float norm = 0.0;

   for (i = 0; i < rows; i++) {
       for (j = 0; j < cols; j++) {
           sum  += weights[i][j] * ( (m2->x[i][j] * m1->x[i][j]) + (m2->y[i][j] * m1->y[i][j]) );
           norm += weights[i][j];
       }
   }
   return (sum/norm);
}

/*************************************************************************
 * Function: compute the diference between the KLT movement (m1) and the *
 * expected movement (m2) and calculates the least square method to      *
 * determine the residue.                                                *
 * Out parameters: the residue.                                          *
 ************************************************************************/
float least_square (Matrix *m1, Matrix *m2, float **weights, int rows, int cols)
{
   int i, j;
   float sum = 0.0;
   Matrix *tmp;

   tmp = rnxm_diff (m1, m2, rows, cols );

   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
         sum  += ( weights[i][j] * ( (tmp->x[i][j] * tmp->x[i][j]) + (tmp->y[i][j] * tmp->y[i][j]) ) );
      }
   }

   disalloc_matrix_model (tmp, rows);
   
   return sum;
}

/***********************************************************************************
 * compute the optical flow of an image through the feature points returned of KLT *
 * Parameters -                                                                    *
 * actual_fl   : feature list of the actual input image.                           *
 * previous_fl : feature list of the previous input image.                         *
 * weights     : weights of each position on the grid.                             *
 * obs: for lost window the displacement in x,y directions and weight is zero.     *
 **********************************************************************************/
Matrix * klt_optical_flow (KLT_FeatureList actual_fl, KLT_FeatureList previous_fl, float **weights, int rows, int cols) 
{
    int i, j, k;

    Matrix *f;

    f  = alloc_matrix_model ( cols, rows );

    for (i = 0, k = 0; i < rows; i++) {
        for (j = 0; j < cols; j++, k++) {
            if (actual_fl->feature[k]->val == KLT_TRACKED) {
                f->x[i][j] = (actual_fl->feature[k]->x - previous_fl->feature[k]->x);
                f->y[i][j] = -(actual_fl->feature[k]->y - previous_fl->feature[k]->y);
            }
            else {
                f->x[i][j] = f->y[i][j] = weights[i][j] = 0.0;
            }
        }
    }

    if (k != (rows * cols)) {
        printf("\n\nerror: the size of the optical flow matrix and the number of the features differ\n");
    }
 
    return f;
}

/**************************************************    
 * Function: store the zero value in all position *
 * of the x,y matrix model.                       *
 * Out parameters: a zeroed matrix model.         *
 *************************************************/
void clean_model (Matrix *model, int rows, int cols)
{
    int i, j;

    for (i = 0; i < rows; i++) {
       for (j = 0; j < cols; j++) {
           model->x[i][j] = model->y[i][j] = 0;
       }
    }
}

/*Matrix * pan_model (int rows, int cols)
{
    int i, j;
    Matrix *m;

    m  = alloc_matrix_model ( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            m->x[i][j] = 1;
            m->y[i][j] = 0;
        }
    }
    return m;
}



Matrix * tilt_model (int rows, int cols)
{
    int i, j;
    Matrix *m;

    m  = alloc_matrix_model ( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            m->x[i][j] = 0;
            m->y[i][j] = 1;
        }
    }
    return m;
}

Matrix * zoom_model (int grows, int gcols, int irows, int icols)
{
    int k, z;
    float x, y;
    float diag;
    float step_r, step_c;

    float h_ir = (float)irows/2.0;
    float h_ic = (float)icols/2.0;

    step_r = (float) irows / (float)(grows-1);
    step_c = (float) icols / (float)(gcols-1);

    Matrix *m;

    m  = alloc_matrix_model ( gcols, grows );
    
    diag = diagonal (irows, icols);

    for (k = 0, y = 0; k < grows; y += step_r, k++) {  
       for (z = 0, x = 0; z < gcols; x += step_c, z++)  {
            m->x[k][z] = (float)  (x - h_ic + 0.5) / (float)diag;
            m->y[k][z] = (float)  (y - h_ir + 0.5) / (float)diag;
       }
    }

    return m;
}


Matrix * roll_model (int grows, int gcols, int irows, int icols)
{
    int   k, z;  
    float x, y;
    float diag;
    float step_r, step_c;

    float h_ir = (float)irows/2.0;
    float h_ic = (float)icols/2.0;

    step_r = (float) irows / (float)(grows-1);
    step_c = (float) icols / (float)(gcols-1);

    Matrix *m;

    m  = alloc_matrix_model ( gcols, grows );
    
    diag = diagonal (irows, icols);

    for (k = 0, y = 0; k < grows; y += step_r, k++) {  
       for (z = 0, x = 0; z < gcols; x += step_c, z++)  {
            m->x[k][z] = (float)  (y - h_ir + 0.5) / (float)diag;
            m->y[k][z] = (float) -(x - h_ic + 0.5) / (float)diag;
       }
    }

    return m;
}*/




/******************************************************************    
 * Function: compute one matrix of x,y that describe a pan model  *
 * all the vectors are horizontal aligned.                        *
 * Out parameters: the pan matrix model.                          *
 *****************************************************************/
Matrix * pan_model (int rows, int cols)
{
    int i, j;
    Matrix *m;

    m  = alloc_matrix_model ( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            m->x[i][j] = -1;
            m->y[i][j] = 0;
        }
    }
    return m;
}

/******************************************************************    
 * Function: compute one matrix of x,y that describe a tilt model *
 * all the vectors are vertical aligned.                          *
 * Out parameters: the tilt matrix model.                         *
 *****************************************************************/
Matrix * tilt_model (int rows, int cols)
{
    int i, j;
    Matrix *m;

    m  = alloc_matrix_model ( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            m->x[i][j] = 0;
            m->y[i][j] = 1;
        }
    }
    return m;
}


/******************************************************************    
 * Function: compute one matrix of x,y that describe a zoom model *
 * all the vectors start on the center of the grid and goes to    *
 * almost all directions.                                         *
 * Out parameters: the zoom matrix model.                         *
 *****************************************************************/
Matrix * zoom_model (int grows, int gcols, int irows, int icols)
{
    int k, z;
    float x, y;
    float diag;
    float step_r, step_c;

    float h_ir = (float)irows/2.0;
    float h_ic = (float)icols/2.0;

    step_r = (float) irows / (float)(grows-1);
    step_c = (float) icols / (float)(gcols-1);

    Matrix *m;

    m  = alloc_matrix_model ( gcols, grows );
    
    diag = diagonal (irows, icols);

    for (k = 0; k < grows; k++) {  
        for (z = 0; z < gcols; z++) {
            x = z*step_c - (h_ic + 0.5);
            y = (h_ir + 0.5) - k*step_r;
            m->x[k][z] = (float)  x / (float)diag;
            m->y[k][z] = (float)  y / (float)diag;
        }
    }
    return m;
}

/******************************************************************    
 * Function: compute a x,y matrix that describe a roll model.     *
 * All the vectors start on the center of the grid and makes a    *
 * circulatory flow.                                              *
 * Out parameters: the roll matrix model.                         *
 *****************************************************************/
Matrix * roll_model (int grows, int gcols, int irows, int icols)
{
    int   k, z;  
    float x, y;
    float diag;
    float step_r, step_c;

    float h_ir = (float)irows/2.0;
    float h_ic = (float)icols/2.0;

    step_r = (float) irows / (float)(grows-1);
    step_c = (float) icols / (float)(gcols-1);

    Matrix *m;

    m  = alloc_matrix_model ( gcols, grows );
    
    diag = diagonal (irows, icols);

    for (k = 0; k < grows; k++) {  
       for (z = 0; z < gcols; z++)  {
            x = z*step_c - (h_ic + 0.5);
            y = (h_ir + 0.5) - k*step_r;
            m->x[k][z] = (float)  y / (float)diag;
            m->y[k][z] = (float) -x / (float)diag;
       }
    }
    return m;
}

/***********************************************    
 * Function: calculates the radial of an image *
 * Out parameters: radial value.               *
 **********************************************/
float diagonal (int rows, int cols)
{ 
    float diagonal = 0;

    float hr = (float)rows / 2; 
    float hc = (float)cols / 2; 

    diagonal = hypotf (hr, hc);

    return diagonal;     
}

/****************************************************************    
 * Function: compute the difference between two matrices of x,y *
 * Out parameters: one matrix with the difference.              *
 ***************************************************************/
Matrix * rnxm_diff (Matrix *m1, Matrix *m2, int rows, int cols )
{
    int i, j;
    Matrix *tmp;
    
    tmp = alloc_matrix_model ( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) { 
           tmp->x[i][j]  = m1->x[i][j] - m2->x[i][j];
           tmp->y[i][j]  = m1->y[i][j] - m2->y[i][j];
        }
    }
    return tmp;
}

/**************************************************************    
 * Function: multiplies one matrix by a scalar.               *
 * Out parameters: one matrix multiplied by scalar.           *
 *************************************************************/
Matrix * rnxm_scale_mult (Matrix *m, double scalar, int rows, int cols)
{
    int i, j;
    Matrix *tmp;

    tmp = alloc_matrix_model( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) { 
           tmp->x[i][j]  = scalar * m->x[i][j];
           tmp->y[i][j]  = scalar * m->y[i][j];
        }
    }
    return tmp;
}

/**************************************************************    
 * Function: sum three matrices of x,y, if the weight of each *
 * position is different of zero.                             *
 * Out parameters: the sum of the three matrices.             *
 *************************************************************/
Matrix * rnxm_sum (Matrix *m1, Matrix *m2, Matrix *m3, Matrix *m4, float **weights, int rows, int cols)
{
    int i, j;
    Matrix *tmp;

    tmp = alloc_matrix_model ( cols, rows );

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (weights[i][j] == 0) {
                tmp->x[i][j] = tmp->y[i][j] = 0;
            }
            else { 
                tmp->x[i][j] = (m1->x[i][j] + m2->x[i][j] + m3->x[i][j] + m4->x[i][j]);
                tmp->y[i][j] = (m1->y[i][j] + m2->y[i][j] + m3->y[i][j] + m4->y[i][j]);
            }
        }
    }
    return tmp;
}

/***************************************************
 * Function: allocate a matrix of dimension h x w  *
 * In parameters: height and width for the matrix  *
 * Out parameters: pointer for matrix of float     *
 ***************************************************/
float ** alloc_mat_float ( int w, int h ) {
    float **m;     
    int   row;     

    m = ( float ** )malloc( h * sizeof( float * ) );
    if ( m == NULL ){  
        fprintf( stderr, "error: allocating array" );
        return NULL;
    }

    for ( row = 0; row < h; row++ ){
        m[row] = ( float * )malloc( w * sizeof( float ) );
        if ( m[row] == NULL ){
            fprintf( stderr, "error: allocating array" );
            return NULL;
        }
    }
    return m; 
}

/***************************************************
 * Function: free a matrix of dimension h x w      *
 * In parameters: matrix and his height            *
 ***************************************************/
void free_mat_float ( float **mat, int h ) {
    int  row; 

    for ( row = 0; row < h; row++ ) {
        free(mat[row]);
    }

    free (mat);
}

/*******************************************************************************
 * Function: write to output file the three values of movement p, t, and z and *
 * the residue between the estimate movement and the KLT momevement,           *
 * In parameters: output file, current frame, previous frame, vector with the  *
 * p, t, and z values, residue and the sum of all the weights of the window.   *
 ******************************************************************************/
void write_to_file (FILE *f, int act_frame, int prv_frame, double *eq_sol, float residue, float s_wts) 
{
    //fprintf(f, "%06d %06d %f %f %f %f %f\n", prv_frame, act_frame, eq_sol[0], eq_sol[1], eq_sol[2], eq_sol[3], sqrtf(residue/s_wts)); 
    fprintf(f, "%f %f %f %f\n", eq_sol[0], eq_sol[1], eq_sol[2], eq_sol[3]); 
    printf("Pan: %f,     Tilt: %f,     Zoom: %f,     Roll: %f\n", eq_sol[0], eq_sol[1], eq_sol[2], eq_sol[3]); 
}

void SetCPUTime(clock_t* value)
{
    *value = clock();
}

double GetElapsedCPUTime()
{
    return ((double) (CPUend - CPUstart)) / CLOCKS_PER_SEC;
}


void print_klt_flow (Matrix *m1, float **weights, int rows, int cols, int iter)
{
   int i, j;
   FILE *file;

   file = fopen(file_name ("kltflow_", "", ".txt", iter), "w");

   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
            fprintf(file, "%f %f ", m1->x[i][j], m1->y[i][j]);       
       }
       fprintf(file, "%s\n", "");
   }
   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
            fprintf(file, "%f ", weights[i][j]);       
       }
       fprintf(file, "%s\n", "");
   }

   fclose(file);
}

void print_flows (Matrix *m1, Matrix *m2, float **weights, float **kappa, float **stwj, int rows, int cols, int frame, int jump, char *name)
{
   int i, j;
   FILE *f;
   f = fopen ("flows.txt", "w");

   fprintf(f,"%s %s\n", file_name (name, "", ".pgm", frame - jump), file_name (name, "", ".pgm", frame)); 
 
   fprintf(f,"%d %d\n", rows, cols);

   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
            fprintf(f, "%f %f ", m1->x[i][j], m1->y[i][j]);       
       }
       fprintf(f, "%s\n", "");
   }

   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
           fprintf(f, "%f %f ", m2->x[i][j], m2->y[i][j]);       
       }
       fprintf(f, "%s\n", "");
   }

   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
           fprintf(f, "%f ", weights[i][j]);
       }
       fprintf(f, "%s\n", "");
   }

   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
           fprintf(f, "%f ", kappa[i][j]);
       }
       fprintf(f, "%s\n", "");
   }

   for(i = 0; i < rows; i++)
   {
       for(j = 0; j < cols; j++)
       {
           fprintf(f, "%f ", stwj[i][j]);
       }
       fprintf(f, "%s\n", "");
   }
   fclose (f);
}
