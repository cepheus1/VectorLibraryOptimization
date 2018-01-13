/* Do not change this file; the task is to optimize vectors.c, not this */

#define _ISOC11_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "vectors.h"

void init(double a[], size_t n, double start)
{
  size_t i;
  for (i=0; i<n; i++) {
    a[i] = start;
    start += 1.0;
  }
}

void printmatrix(double a[], size_t ncols, size_t nrows)
{
  size_t i,j;
  for (j=0; j<nrows; j++) {
    for (i=0; i<ncols; i++) {
      printf("%7.3g ", a[j*ncols+i]);
    }
    printf("\n");
  }
}

/* a has m cols and n rows, b has p cols and m rows, c has p cols and n rows */
void matmul(double a[], double b[], double c[], size_t m, size_t n, size_t p)
{
  size_t i,k;
  Vector *vb, *vc;
  vb = calloc(m,sizeof(Vector *));
  for (i=0; i<m; i++) {
    vassign(vb[i], vnew(&b[i*p],p));
  }
  vc = calloc(n,sizeof(Vector *));
  memset(c,0,p*sizeof(double));
  for (i=0; i<n; i++) {
    vassign(vc[i], vnew(c,p));
  }
  for (i=0; i<n; i++)
    for (k=0; k<m; k++)
      vassign(vc[i],vsum(vconsume(vc[i]),svprod(a[i*m+k],vcopy(vb[k]))));
  for (i=0; i<n; i++)
    vstore(c+i*p, p, vconsume(vc[i]));
}

int main(int argc, char *argv[])
{
  if (argc!=2) {
    fprintf(stderr,"Usage: %s n\n",argv[0]);
    exit(1);
  }
  size_t d = atol(argv[1]);
  size_t m = d;
  size_t n = d;
  size_t p = 99;
  // size_t m = 2;
  // size_t n = 3;
  // size_t p = 4;
  double *a=malloc(m*n*sizeof(double));
  double *b=malloc(m*p*sizeof(double));
  double *c=malloc(n*p*sizeof(double));
  FILE *f = fopen("matmul.out","wb");
  init(a,m*n,1.0);
  // init(b,p*m,8.0);
  init(b,p*m,50000.0);
  matmul(a,b,c,m,n,p);
  //printmatrix(c,p,n);
  fwrite(c, sizeof(double), n*p, f);
  fclose(f);
  return 0;
}
