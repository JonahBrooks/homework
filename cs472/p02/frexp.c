#include "frexp.h"
#include <math.h>
#include <stdio.h>

#define SIGN_MASK 0x8000000000000000
#define SIGN_SHIFT 63
#define EXP_MASK  0x7ff0000000000000
#define EXP_SHIFT  52
#define EXP_BIAS 1023
#define FRAC_MASK 0x000fffffffffffff
#define FRAC_SHIFT 0

double myfrexp(double x, int *exp)
{

  union dandi_union
  {
    double d;
    long unsigned int i;
    unsigned char c[8];
  } dandi, dandi2;

  if(x == 0) // or x == -0
  { 
    (*exp) = 0;
    return 0.0;
  } 

  if(x != x) // x is NaN
  {
    (*exp) = 0; // Or anything else. exp is undefined in this case.
    return x; // return NaN
  }

  if(!isfinite(x)) // x is +INFINITY or -INFINITY
  {
    (*exp) = 0; // Or anything else. exp is undefined in this case.
    return x; // return +/- INFINITY
  }

  // if x is not a special case:
  dandi2.d = x;
  int i;
  i = 0;
  for(i = 0; i < 8; i++)
  {
    dandi.c[i] = dandi2.c[7-i];
   // dandi.c[i] = dandi2.c[i];
  }
  for(i = 0; i < 8; i++)
  {
    printf("1: %02x\t2: %02x\n",dandi.c[i],dandi2.c[i]);
  }
  // Now dandi2 is in system endianness and dandi is reverse of that

  double fraction;
  //fraction = (dandi.i & FRAC_MASK) >> FRAC_SHIFT;
  fraction =  (( 0x10 * 256*7) +
              ((dandi.c[1] & 0x0f) * 256*7) +
              ((dandi.c[2] & 0xff) * 256*6) +
              ((dandi.c[3] & 0xff) * 256*5) + 
              ((dandi.c[4] & 0xff) * 256*4) + 
              ((dandi.c[5] & 0xff) * 256*3) + 
              ((dandi.c[6] & 0xff) * 256*2) + 
              ((dandi.c[7] & 0xff) * 256*1) );
  fraction = fraction / 0xfffffffffffff;
  int exponent;
  //exponent = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  exponent = (((dandi.c[0] & 0x7f) << 8) + (dandi.c[1] & 0xf0)) >> 4;
  exponent -= EXP_BIAS;
  exponent += 1; // Adjust for including the implicit 1 in the fraction
  int sign;
  sign = (dandi.c[0] & 0x80) >> 7;
  //sign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  if(sign == 0)
  {
    sign = 1;
  } 
  else
  {
    sign = -1;
  }

  (*exp) = exponent;
  return (sign*(fraction));
  


}
