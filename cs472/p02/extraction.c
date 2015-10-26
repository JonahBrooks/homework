#include "extraction.h"
#include "hw2.h"
#include <math.h>
#include <stdio.h>


void extraction(double x)
{
  int endiflip;
  // TODO: Set this by some sort of endinness check
  // Determine whether a conversion between little and big endian needs to happen
  endiflip = 0; // false


//  union dandi_union
//  {
//    double d;
//    long unsigned int u;
//    long int i;
//    unsigned char c[8];
//  } dandi, dandi2;

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
  double mantissa;
  mantissa = ((dandi.i & FRAC_MASK) >> FRAC_SHIFT) / pow(2.0,52);
  int sign;
  sign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  sign = (sign) ? -1 : 1;

  printf("\nThe double value %f (bit pattern 0x%02x%02x%02x%02x%02x%02x%02x%02x) represents the following:\n",
          dandi2.d, dandi2.c[0], dandi2.c[1], dandi2.c[2], dandi2.c[3], 
          dandi2.c[4], dandi2.c[5], dandi2.c[6], dandi2.c[7]); 
  printf("sign: %d (0x%lx)\texponent: %d (0x%lx) \tmantissa: %f (0x%lx)\n", 
          sign, ((dandi2.u & SIGN_MASK) >> SIGN_SHIFT), 
          exponent, ((dandi2.u & EXP_MASK) >> EXP_SHIFT), 
          mantissa, ((dandi2.u & FRAC_MASK) >> FRAC_SHIFT));
  printf("As a long: %ld\n", dandi2.i);
  printf("As characters: \"%c%c%c%c%c%c%c%c\"\n",
          dandi2.c[0], dandi2.c[1], dandi2.c[2], dandi2.c[3], 
          dandi2.c[4], dandi2.c[5], dandi2.c[6], dandi2.c[7]); 
}
