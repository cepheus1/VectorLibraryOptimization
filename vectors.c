/* Simple implementation where every malloc()ed object has exactly one
   reference */

#include "vectors.h"
#include <stdbool.h>

struct vect_private {
  double factor;
  size_t n;
  char padding[16]; // ensures that d is 32 byte aligned
  double d[0];
};

VPriv last_vector;
bool last_vector_free;

VPriv allocate_vector(size_t n)
{
  size_t vbytes = sizeof(struct vect_private) + ((n + 3) & ~3UL) * sizeof(double);
  vbytes = (vbytes + 31) & ~31UL;

  if (!last_vector_free || last_vector->n != n)
    __builtin_posix_memalign((void **)&last_vector, 32, vbytes);
  
  last_vector_free = false;
  return last_vector;
}

void free_vector(VPriv v)
{
  if (v == NULL)
    return;

  if (last_vector == v)
    last_vector_free = true;
  else
    free(v);
}

VPriv vcopy(Vector v)
{
  VPriv v1 = (VPriv)v;

  VPriv r = allocate_vector(v1->n);
  
  size_t vbytes = sizeof(struct vect_private)+((v1->n + 3) & ~3UL)*sizeof(double);
  vbytes = (vbytes + 31) & ~31UL;
  
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
    free_vector(v1);
    vp = NULL;
  }
}

void vassign1(Vector *vp, VPriv v)
{
  VPriv v1 = (VPriv)*vp;
  if (v1 != NULL)
    free_vector(v1);
  *vp = (Vector)v;
}

/* Vector sum and product */

VPriv vsum(VPriv restrict v1, VPriv restrict v2)
{
  size_t i;
  if (v1->n != v2->n) {
    fprintf(stderr,"vsum: vector size mismatch\n");
    exit(1);
  }
  /* We know that this operation consumes v1 and v2, so we can reuse
     the memory of v1 for the result */
  if (v1->factor != 1.0) {
    for (i=0; i<v1->n; i++)
      v1->d[i] = v1->factor * v1->d[i] + v2->factor * v2->d[i];
    v1->factor = 1.0;
  } else {
    double *d1 = __builtin_assume_aligned(v1->d, 32);
    double *d2 = __builtin_assume_aligned(v2->d, 32);
    size_t n = (v1->n + 3) & ~3UL;
    for (i=0; i<n; i++)
      d1[i] += v2->factor * d2[i];
  }
  free_vector(v2);
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
  VPriv v = allocate_vector(n);
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
  free_vector(v);
}
