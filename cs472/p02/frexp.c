#include "frexp.h"
#include <math.h>
#include <stdio.h>

// TODO: I'd like to switch things around to use these again...

#define SIGN_MASK 0x8000000000000000
#define SIGN_SHIFT 63
#define EXP_MASK  0x7ff0000000000000
#define EXP_SHIFT  52
#define EXP_BIAS 1023
#define FRAC_MASK 0x000fffffffffffff
#define FRAC_SHIFT 0

double myfrexp(double x, int *exp)
{

  int endiflip;
  // TODO: Set this by some sort of endinness check
  // Determine whether a conversion between little and big endian needs to happen
  endiflip = 0; // false

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
  dandi.d = x;
  dandi2.d = x;
  int i;
  i = 0;
  if(endiflip)
  {
    for(i = 0; i < 8; i++)
    {
      dandi.c[i] = dandi2.c[7-i];
      // Now dandi2 is in system endianness and dandi is reverse of that
    }
  }

  int exponent;
  exponent = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  exponent -= EXP_BIAS;
  exponent += 1; // Adjust for including the implicit 1 in the fraction
  double fraction;
  fraction = (dandi.i & FRAC_MASK) >> FRAC_SHIFT;
  fraction = fraction / pow(2.0,53.0);
  fraction += 0.5;
  int sign;
  sign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  sign = (sign) ? -1 : 1;

  (*exp) = exponent;
  return (sign*(fraction));

}
