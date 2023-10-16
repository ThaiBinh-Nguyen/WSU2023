/* Full Name: Thai Binh Nguyen
   Course: CS330
   Project 2: LU Factorization Engine
   Date: 10-16-2023
*/

/* Identifies the project:
  My Project will be creating a C99 library for solving linear equations of the form Ax = b using 
  LU factorization with partial pivoting using C programming language.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LUfact.h"

double **createMatrix(int N) {
  double **M = (double **) malloc(N * sizeof(double*));
  for (int i = 0; i < N; i++)
    M[i] = (double*) malloc(N * sizeof(double));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      M[i][j] = (i == j) ? 1.0 : 0.0;
  return M;
}

void destroyMatrix(int N, double **M) {
  for (int i = 0; i < N; i++)
    free(M[i]);
  free(M);
}

LUfact *LUfactor(int N, const double **A) {
  // Initialize LUfact struct
  LUfact *LU = (LUfact *) malloc(sizeof(LUfact));
  LU->N = N;
  LU->LU = createMatrix(N);
  LU->mutate = (short *) malloc(N * sizeof(short));

  // Copy A into LU and initialize mutate
  for (int i = 0; i < N; i++) {
    LU->mutate[i] = i;
    for (int j = 0; j < N; j++) {
      LU->LU[i][j] = A[i][j];
    }
  }

  // Perform LU decomposition with partial pivoting
  for (int k = 0; k < N; k++) {
    // Find pivot
    double max_val = fabs(LU->LU[k][k]);
    int pivot = k;
    for (int i = k + 1; i < N; i++) {
      if (fabs(LU->LU[i][k]) > max_val) {
        max_val = fabs(LU->LU[i][k]);
        pivot = i;
      }
    }

    // Swap rows
    if (pivot != k) {
      short tmp = LU->mutate[k];
      LU->mutate[k] = LU->mutate[pivot];
      LU->mutate[pivot] = tmp;

      double *tmp_ptr = LU->LU[k];
      LU->LU[k] = LU->LU[pivot];
      LU->LU[pivot] = tmp_ptr;
    }

    // Check if matrix is singular
    if (fabs(LU->LU[k][k]) < 1e-8) {
      destroyMatrix(N, LU->LU);
      free(LU->mutate);
      free(LU);
      return NULL;
    }

    // Eliminate below diagonal
    for (int i = k + 1; i < N; i++) {
      LU->LU[i][k] /= LU->LU[k][k];
      for (int j = k + 1; j < N; j++) {
        LU->LU[i][j] -= LU->LU[i][k] * LU->LU[k][j];
      }
    }
  }
  return LU;
}

void LUdestroy(LUfact *fact) {
  if (fact) {
    destroyMatrix(fact->N, fact->LU);
    free(fact->mutate);
    free(fact);
  }
}

void LUsolve(LUfact *fact, const double *b, double *x) {
  int N = fact->N;

  // Forward substitution for L
  for (int i = 0; i < N; i++) {
    x[i] = b[fact->mutate[i]];
    for (int j = 0; j < i; j++) {
      x[i] -= fact->LU[i][j] * x[j];
    }
  }

  // Backward substitution for U
  for (int i = N - 1; i >= 0; i--) {
    for (int j = i + 1; j < N; j++) {
      x[i] -= fact->LU[i][j] * x[j];
    }
    x[i] /= fact->LU[i][i];
  }
}
