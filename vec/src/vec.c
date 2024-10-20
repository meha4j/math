#include <vec.h>

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

struct vec* vec_new(int n) {
  struct vec* vec = malloc(sizeof(struct vec));

  vec->v = malloc(sizeof(real) * n);
  vec->n = n;

  return vec;
}

void vec_rnd(struct vec* vp, int u) {
  int n = vp->n;
  real* vvp = vp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    vvp[i] = rand() % (u + 1);
}

void vec_seq(struct vec* vp, int s) {
  int n = vp->n;
  real* vvp = vp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0, v = s; i < n; ++i, ++v)
    vvp[i] = v;
}

int vec_fget(FILE* f, struct vec* vp) {
  int n = vp->n;
  real* vvp = vp->v;

  for (int i = 0; i < n; ++i)
    if (fscanf(f, FMT, &vvp[i]) != 1)
      return -1;

  return 0;
}

int vec_fput(FILE* f, struct vec* vp) {
  int n = vp->n;
  real* vvp = vp->v;

  for (int i = 0; i < n; ++i)
    if (fprintf(f, "%.3e ", vvp[i]) < 0)
      return -1;

  return 0;
}

void vec_add(struct vec* ap, struct vec* bp, struct vec* rp) {
  int n = ap->n;

  real* avp = ap->v;
  real* bvp = bp->v;
  real* rvp = rp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    rvp[i] = avp[i] + bvp[i];
}

void vec_cmb(struct vec* ap, struct vec* bp, struct vec* rp, real k) {
  int n = ap->n;

  real* avp = ap->v;
  real* bvp = bp->v;
  real* rvp = rp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    rvp[i] = avp[i] + bvp[i] * k;
}

void vec_mlt(struct vec* ap, struct vec* bp, real* rp) {
  int n = ap->n;

  real* avp = ap->v;
  real* bvp = bp->v;
  real s = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    s += avp[i] * bvp[i];

  *rp = s;
}

void vec_nrm(struct vec* vp, double* rp) {
  int n = vp->n;

  real* vvp = vp->v;
  synt s = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    s += vvp[i] * vvp[i];

  *rp = sqrt(s);
}

void vec_free(struct vec* vp) {
  free(vp->v);
  free(vp);
}
