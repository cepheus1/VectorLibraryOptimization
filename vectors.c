/* Simple implementation where every malloc()ed object has exactly one
   reference */

#include "vectors.h"

struct vect_private {
  double factor;
  size_t n;
  double d[0];
};

VPriv vcopy(Vector v)
{
  VPriv v1 = (VPriv)v;
  size_t vbytes = sizeof(struct vect_private)+v1->n*sizeof(double);
  VPriv r = malloc(vbytes);
  memcpy(r,v1,vbytes);
  return r;
}

VPriv vconsume1(Vector *vp)
{
   VPriv r = (VPriv)*vp;
   *vp = NULL;
   return r;
}

void vdelete1(Vector *vp)
{
  VPriv v1 = (VPriv)*vp;
  if (v1 != NULL) {
    free(v1);
    vp = NULL;
  }
}

void vassign1(Vector *vp, VPriv v)
{
  VPriv v1 = (VPriv)*vp;
  if (v1 != NULL)
    free(v1);
  *vp = (Vector)v;
}

/* Vector sum and product */

VPriv vsum(VPriv v1, VPriv v2)
{
  size_t i;
  if (v1->n != v2->n) {
    fprintf(stderr,"vsum: vector size mismatch\n");
    exit(1);
  }
  /* We know that this operation consumes v1 and v2, so we can reuse
     the memory of v1 for the result */
  for (i=0; i < v1->n-3; i+=4) {
    v1->d[i] = v1->factor * v1->d[i] + v2->factor * v2->d[i];
    v1->d[i+1] = v1->factor * v1->d[i+1] + v2->factor * v2->d[i+1];
    v1->d[i+2] = v1->factor * v1->d[i+2] + v2->factor * v2->d[i+2];
    v1->d[i+3] = v1->factor * v1->d[i+3] + v2->factor * v2->d[i+3];
  }
  for (; i < v1->n; i++) {
    v1->d[i] = v1->factor * v1->d[i] + v2->factor * v2->d[i];
  }
  free(v2);
  return v1;
}

VPriv svprod(double d, VPriv v1)
{
  v1->factor *= d;
  return v1;
}

/* Conversion between abstract vectors and concrete memory arrays */
/* Create a vector with n elements starting at dp */
VPriv vnew(double *dp, size_t n)
{
  size_t vbytes = sizeof(struct vect_private)+n*sizeof(double);
  VPriv v = malloc(vbytes);
  v->n = n;
  v->factor = 1.0;
  memcpy(v->d,dp,n*sizeof(double));
  return v;
}
/* Store the elements from v at sp; The number of elements in v must be n */
void vstore(double *dp, size_t n, VPriv v)
{
  if (v->n != n) {
    fprintf(stderr,"vstore: vector size mismatch\n");
    exit(1);
  }
  for (size_t i = 0; i < v->n; i++)
    v->d[i] *= v->factor;
  memcpy(dp,v->d,n*sizeof(double));
  free(v);
}
