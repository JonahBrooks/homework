#include "extraction.h"
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

void extraction(double x)
{

  union dandi_union
  {
    double d;
    long unsigned int u;
    long int i;
    unsigned char c[8];
  } dandi, dandi2;

  dandi2.d = x;
  int i;
  i = 0;
  for(i = 0; i < 8; i++)
  {
    dandi.c[i] = dandi2.c[7-i];
   // dandi.c[i] = dandi2.c[i];
  }
  //for(i = 0; i < 8; i++)
  //{
  //  printf("1: %02x\t2: %02x\n",dandi.c[i],dandi2.c[i]);
  //}
  // Now dandi2 is in system endianness and dandi is reverse of that

  //dandi.d = x;

  int exponent;
  //exponent = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  exponent = (((dandi.c[0] & 0x7f) << 8) + (dandi.c[1] & 0xf0)) >> 4;
  exponent -= EXP_BIAS;
  double mantissa;
  //fraction = (dandi.i & FRAC_MASK) >> FRAC_SHIFT;
  mantissa =  (((dandi.c[1] & 0x0f) / (double)(2<<3)) +
              ((dandi.c[2] & 0xff) / (double)(2<<11)) +
              ((dandi.c[3] & 0xff) / (double)(2<<19)) + 
              ((dandi.c[4] & 0xff) / (double)(2<<27)) + 
              ((dandi.c[5] & 0xff) / pow(2.0,35.0)) + 
              ((dandi.c[6] & 0xff) / pow(2.0,43.0)) + 
              ((dandi.c[7] & 0xff) / pow(2.0,51.0)) );
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

  printf("\nThe double value %f (bit pattern 0x%02x%02x%02x%02x%02x%02x%02x%02x) represents the following:\n",
          dandi2.d, dandi2.c[0], dandi2.c[1], dandi2.c[2], dandi2.c[3], 
          dandi2.c[4], dandi2.c[5], dandi2.c[6], dandi2.c[7]); 
  printf("sign: %d\texponent: %d\tmantissa: %f\n", sign, exponent, mantissa);     
  printf("As a long: %ld\n", dandi2.i);
  printf("As characters: \"%c%c%c%c%c%c%c%c\"\n",
          dandi2.c[0], dandi2.c[1], dandi2.c[2], dandi2.c[3], 
          dandi2.c[4], dandi2.c[5], dandi2.c[6], dandi2.c[7]); 
}
