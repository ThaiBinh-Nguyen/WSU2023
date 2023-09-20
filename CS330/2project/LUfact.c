#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LUfact.h"

double **createMatrix(int N) {
  double **M = (double **) malloc(N*sizeof(double*));
  for (int i = 0; i < N; i++)
    M[i] = (double*) malloc(N*sizeof(double));
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
  LUfact *LU = (LUfact*) malloc(sizeof(LUfact));
  LU->N = N;
  LU->LU = createMatrix(N);
  LU->mutate = (short *) malloc(N*sizeof(short));

  // Clone A into LU
  double **A_ = LU->LU;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      A_[i][j] = A[i][j];

  for (int i = 0; i < N; i++)
    LU->mutate[i] = (short) i;

  // actual factorizing goes here

  return LU;
}

void LUdestroy(LUfact *fact) {

}

void LUsolve(LUfact *fact, const double *b, double *x) {

}
