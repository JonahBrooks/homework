#include "fops.h"
#include "hw2.h"
#include <math.h>
#include <stdio.h>

double fadd(double x, double y)
{
  double toReturn;
  
  unsigned long int xexp;
  unsigned long int xfrac;
  unsigned long int xsign;
  unsigned long int yexp;
  unsigned long int yfrac;
  unsigned long int ysign;
  unsigned long int rexp;
  unsigned long int rfrac;
  unsigned long int rsign;

  dandi.d = x;
  dandi2.d = y;

  xexp = (dandi.u & EXP_MASK) >> EXP_SHIFT;
  //xexp -= EXP_BIAS-1; // Account for the implicit 1 in the mantissa
  xfrac = ((dandi.u & FRAC_MASK) >> FRAC_SHIFT);
  xsign = (dandi.u & SIGN_MASK) >> SIGN_SHIFT;
  xsign = (xsign) ? -1 : 1;
  
  yexp = (dandi2.u & EXP_MASK) >> EXP_SHIFT;
  //yexp -= EXP_BIAS-1;
  yfrac = ((dandi2.u & FRAC_MASK) >> FRAC_SHIFT);
  ysign = (dandi2.u & SIGN_MASK) >> SIGN_SHIFT;
  ysign = (ysign) ? -1 : 1;

  int expdiff;
  expdiff = xexp - yexp;
  if(xexp < yexp)
  {
    xfrac = xfrac >> abs(expdiff);
    xexp = yexp;
    rexp = yexp;
  }
  else
  {
    yfrac = yfrac >> abs(expdiff);
    yexp = xexp;
    rexp = xexp;
  }
  xfrac = (xfrac | MANT_MASK)>>1; // Add back in the implicit 1 of the mantissa
  yfrac = (yfrac | MANT_MASK)>>1; // Add back in the implicit 1 of the mantissa

  //rexp += EXP_BIAS-1;
  // TODO: Adjust for situations in which the exponent increased
  //flip(&xfrac);
  //flip(&yfrac);
  rfrac = (xfrac + yfrac);
  if ((rfrac & (MANT_MASK)) > 0) // There was a carry
  {
    rexp += 1; 
    rfrac >> 1;
    printf("\nRawr\n");
  }
  rfrac = rfrac & FRAC_MASK;
  //flip(&rfrac);
  // TODO: Figure out rsign
  rsign = xsign;  

  toReturn = 0.0;
  toReturn = ((rsign & SIGN_MASK) << SIGN_SHIFT) | 
             ((rexp & EXP_MASK) << EXP_SHIFT) | 
             ((rfrac & FRAC_MASK) << FRAC_SHIFT);
  //flip(&toReturn);
  dandi2.d = toReturn;
  printf("\n\nx:%lu (0x%016lx)\ny:%lu (0x%016lx)\nx+y:%lu (0x%016lx)",xfrac,xfrac,yfrac,yfrac,rfrac,rfrac);
  printf("\nThe double value %f (bit pattern 0x%02x%02x%02x%02x%02x%02x%02x%02x) represents the following:\n",
          dandi2.d, dandi2.c[0], dandi2.c[1], dandi2.c[2], dandi2.c[3], 
          dandi2.c[4], dandi2.c[5], dandi2.c[6], dandi2.c[7]); 
  printf("sign: %d (0x%lx)\texponent: %lu (0x%lx) \tmantissa: %f (0x%lx)\n", 
          rsign, ((dandi2.u & SIGN_MASK) >> SIGN_SHIFT), 
          (rexp-EXP_BIAS+1), ((dandi2.u & EXP_MASK) >> EXP_SHIFT), 
          ((double)rfrac/pow(2.0,54.0)), ((dandi2.u & FRAC_MASK) >> FRAC_SHIFT));

  //flip(&toReturn);
 // printf("x:%lu (0x%016lx)\ny:%lu (0x%016lx)\n",xfrac,xfrac,yfrac,yfrac);
  
  return toReturn;
}

double fsub(double x, double y)
{
  unsigned long int xexp;
  unsigned long int xfrac;
  unsigned long int xsign;
  unsigned long int yexp;
  unsigned long int yfrac;
  unsigned long int ysign;

  dandi.d = x;
  dandi2.d = y;

  xexp = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  xexp -= EXP_BIAS-1;
  xfrac = ((dandi.i & FRAC_MASK) >> FRAC_SHIFT);
  xsign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  xsign = (xsign) ? -1 : 1;
  
  yexp = (dandi2.i & EXP_MASK) >> EXP_SHIFT;
  yexp -= EXP_BIAS-1;
  yfrac = ((dandi2.i & FRAC_MASK) >> FRAC_SHIFT);
  ysign = (dandi2.i & SIGN_MASK) >> SIGN_SHIFT;
  ysign = (ysign) ? -1 : 1;

  return 0.0;
}

double fmul(double x, double y)
{
  unsigned long int xexp;
  unsigned long int xfrac;
  unsigned long int xsign;
  unsigned long int yexp;
  unsigned long int yfrac;
  unsigned long int ysign;

  dandi.d = x;
  dandi2.d = y;

  xexp = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  xexp -= EXP_BIAS-1;
  xfrac = ((dandi.i & FRAC_MASK) >> FRAC_SHIFT);
  xsign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  xsign = (xsign) ? -1 : 1;
  
  yexp = (dandi2.i & EXP_MASK) >> EXP_SHIFT;
  yexp -= EXP_BIAS-1;
  yfrac = ((dandi2.i & FRAC_MASK) >> FRAC_SHIFT);
  ysign = (dandi2.i & SIGN_MASK) >> SIGN_SHIFT;
  ysign = (ysign) ? -1 : 1;

  return 0.0;
}

double fdiv(double x, double y)
{
  unsigned long int xexp;
  unsigned long int xfrac;
  unsigned long int xsign;
  unsigned long int yexp;
  unsigned long int yfrac;
  unsigned long int ysign;

  dandi.d = x;
  dandi2.d = y;

  xexp = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  xexp -= EXP_BIAS-1;
  xfrac = ((dandi.i & FRAC_MASK) >> FRAC_SHIFT);
  xsign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  xsign = (xsign) ? -1 : 1;
  
  yexp = (dandi2.i & EXP_MASK) >> EXP_SHIFT;
  yexp -= EXP_BIAS-1;
  yfrac = ((dandi2.i & FRAC_MASK) >> FRAC_SHIFT);
  ysign = (dandi2.i & SIGN_MASK) >> SIGN_SHIFT;
  ysign = (ysign) ? -1 : 1;

  return 0.0;
}

double fsqr(double x)
{
  unsigned long int xexp;
  unsigned long int xfrac;
  unsigned long int xsign;
  unsigned long int yexp;
  unsigned long int yfrac;
  unsigned long int ysign;

  dandi.d = x;

  xexp = (dandi.i & EXP_MASK) >> EXP_SHIFT;
  xexp -= EXP_BIAS-1;
  xfrac = ((dandi.i & FRAC_MASK) >> FRAC_SHIFT);
  xsign = (dandi.i & SIGN_MASK) >> SIGN_SHIFT;
  xsign = (xsign) ? -1 : 1;
  

  return 0.0;
}

void fops()
{
  double x;
  double y;
  x = 7.0;
  y = 3.0;

  printf("fadd(%f,%f) = %f\n",x,y,fadd(x,y));
  unsigned long int z;
  z = 1;
  printf("z:%lu\t",z);
  flip((void*)(&z));
  printf("s:%lu\n",z);
}
