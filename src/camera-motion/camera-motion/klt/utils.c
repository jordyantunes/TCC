/* 
** From J. Stolfi C functions 
**
** Problem: Calculate de inverse of a nxn matrix.
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

#include "utils.h"

void rmxn_inv (int n, double *A, double *R)
{ 
    int i, j;
    int n2 = n*n;
    double *C = (double *)malloc(2*n2*sizeof(double));
    /* Copy {A} into the left half of {C}, fill the right half with the identity: */
    for (i = 0; i < n; i++) 
    { 
        double *Cij = &(C[2*n*i]); 
        double *Aij = &(A[n*i]);
        for (j = 0; j < n; j++) 
        { 
           (*Cij) = (*Aij); Cij++; Aij++; 
        }
        for (j = 0; j < n; j++) 
        { 
           (*Cij) = (j == i ? 1.0 : 0.0); Cij++; 
        }
    }
    gsel_triangularize(n, 2*n, C, 0);
    gsel_diagonalize(n, 2*n, C, 0);
    gsel_normalize(n, 2*n, C, 0);
    for (i = 0; i < n; i++) 
    { 
        double *Cij = &(C[2*n*i + n]); 
        double *Rij = &(R[n*i]);
        for (j = 0; j < n; j++) 
        { 
           (*Rij) = (*Cij); Rij++; Cij++; 
        }
    }
    free(C);
}

void gsel_triangularize (int m, int n, double M[], int total)
{ 
    int i = 0;
    int j = 0;
    double *Mij = &(M[0]);
    while ((i < m-1) && (j < n))
    { 
        /* Elements {M[r][s]} with {r >= i} and {s < j} are all zero. */
        /* Clear elements {M[k][j]} with {k > i} */
        int k; double *Mkj;
        for (k = i+1, Mkj = Mij + n; k < m; k++, Mkj += n)
        { 
            /* Swap rows {i} and {k} if needed, negating one of them: */
            if (fabs(*Mkj) > fabs(*Mij))
            { 
                int r; double *Mkr; double *Mir;
                for (r = j, Mkr = Mkj, Mir = Mij; r < n; r++, Mkr++, Mir++)
                { 
                    double t = (*Mkr); (*Mkr) = - (*Mir); (*Mir) = t; 
                }
            }
            /* Subtract from row {k} a multiple of row {i} that cancels {M[k,j]}: */
            if ((*Mkj) != 0.0)
            { 
                int r; double *Mkr; double *Mir;
                double s = (*Mkj)/(*Mij);
                (*Mkj) = 0.0;
                for (r = j+1, Mkr = Mkj+1, Mir = Mij+1; r < n; r++, Mkr++, Mir++)
                { 
                    double old = (*Mkr);
                    (*Mkr) = old - s*(*Mir);
                    /* Feeble attempt to clean out entries that are just roundoff error: */
                    if (1.0e12*fabs(*Mkr) < fabs(old)) { (*Mkr) = 0.0; }
                }
            }
        }
        if ((! total) || ((*Mij) != 0.0)) 
        { 
           i++; Mij += n; 
        }
        j++; Mij++;
    }
}

void gsel_diagonalize(int m, int n, double M[], int total)
{ 
    int i = 0;
    int j = 0;
    double *Mij = &(M[0]);
    while ((i < m) && (j < n))
    { 
        if ((*Mij) != 0.0)
        {  
            /* Clear elements {M[k][j]} with {k < i} */
            int k; double *Mkj;
            for (k = i-1, Mkj = Mij-n; k >= 0; k--, Mkj -= n)
            { 
                /* Sub from row {k} a multiple of row {i} that cancels {M[k,j]}: */
                if ((*Mkj) != 0.0)
                { 
                    int r; double *Mkr; double *Mir;
                    double s = (*Mkj)/(*Mij);
                    (*Mkj) = 0.0;
                    for (r = j+1, Mkr = Mkj+1, Mir = Mij+1; r < n; r++, Mkr++, Mir++)
                      { (*Mkr) -= s*(*Mir); }
                }
            }
            i++; Mij += n;
        }
        else if (! total)
        { i++; Mij += n; }
        j++; Mij++;
    }
}

void gsel_normalize(int m, int n, double M[], int total)
{ 
    int i = 0;
    int j = 0;
    double *Mij = &(M[0]);
    while ((i < m) && (j < n))
    { 
        if ((*Mij) != 0.0)
        { 
            /* Scale row {i} by {1/M[i,j]}: */
            int r; double *Mir;
            double s = (*Mij);
            (*Mij) = 1.0;
            for (r = j+1, Mir = Mij+1; r < n; r++, Mir++) { (*Mir) /= s; }
            i++; Mij += n;
        }
        else if (! total)
        { 
           i++; Mij += n; 
        }
        j++; Mij++;
    }
}

void rmxn_map_col (int m, int n, double *A, double *x, double *r)
{ 
    int i, j, t = 0;
    for (i = 0; i < m; i++)
    { 
        double sum = 0.0, corr = 0.0; 
        for (j = 0; j < n; j++)
        { 
            double term = A[t] * x[j]; 
            /* Kahan's summation: */
            double tcorr = term - corr;
            double newSum = sum + tcorr;
            corr = (newSum - sum) - tcorr;
            sum = newSum;
            t++;
        }
        r[i] = sum;
    }
}
