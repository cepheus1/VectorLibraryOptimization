/* vector operations with value semantics, i.e., vectors are
   immutable, a copy of a vector is not affected by operations on the
   original, and operations consume vectors.  Vectors must only be
   processed with the functions/macros in this header, not with
   standard C features like assignment, in order to keep the
   guarantees and avoid memory management problems like memory leaks,
   use-after-free, or double free */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* do not copy Vector values, except by using the functions/macros
   below; Vector variables must be initialized to NULL */
typedef struct vect_public *Vector;

/* do not deal with VPriv at all (no variables etc.), except through
   the functions and macros below */
typedef struct vect_private *VPriv;

/* create a new copy of a vector */
VPriv vcopy(Vector);

/* vconsume(var) prepare the vector for consumption, clearing v */
VPriv vconsume1(Vector *);
#define vconsume(lval) vconsume1(&lval)

/* vdelete(var) clears v (freeing any contained vector if necessary) */
void vdelete1(Vector *);
#define vdelete(lval) vdelete1(&lval)

/* vassign(var,v) stores v in var */
void vassign1(Vector *, VPriv);
#define vassign(lval,vp) vassign1(&lval,vp)

/* Elementwise sum, argument vectors must have the same length */
VPriv vsum(VPriv,VPriv);
/* Elementwise multiplication with a scalar */
VPriv svprod(double,VPriv);

/* Conversion between abstract vectors and concrete memory arrays */
/* Create a vector with n elements starting at dp */
VPriv vnew(double *dp, size_t n);
/* Store the elements from v at sp; The number of elements in v must be n */
void vstore(double *dp, size_t n, VPriv v);
     

  
