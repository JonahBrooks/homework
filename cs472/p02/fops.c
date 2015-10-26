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
  xfrac = ((dandi.u & FRAC_MASK) >> FRAC_SHIFT);
  xsign = (dandi.u & SIGN_MASK) >> SIGN_SHIFT;
  
  yexp = (dandi2.u & EXP_MASK) >> EXP_SHIFT;
  yfrac = ((dandi2.u & FRAC_MASK) >> FRAC_SHIFT);
  ysign = (dandi2.u & SIGN_MASK) >> SIGN_SHIFT;

  // Detect if one or both are 0
  if(yexp == 0 && yfrac == 0) // y is 0
  {
    return x;
  }
  if(xexp == 0 && xfrac == 0) // x is 0
  {
    return y;
  }

  // If not, add the implicit 1 to each mantissa
  xfrac = (xfrac | MANT_MASK); 
  yfrac = (yfrac | MANT_MASK); 

  // Allign the two numbers together
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
  
  rsign = 0; // Until an operation changes something, let's be possitive
  // Add them using the ALU
  if(ysign == 0 && xsign == 1) // x is negative, y is positive
  {
    rfrac = yfrac - xfrac;
    if(xfrac > yfrac)
    {
      rsign = 1; // Result will be negative 
    }
  }
  else if(xsign == 0 && ysign == 1) // y is negative, x is positive
  {
    rfrac = xfrac - yfrac;
    if(yfrac > xfrac)
    {
      rsign = 1; // Result will be negative 
    }
  }
  else //if(xsign == ysign) // both the same sign
  {
    rfrac = (xfrac + yfrac);
    // Shift and increase exponent if needed
    if ((rfrac & (MANT_MASK << 1)) > 0) // There was a carry
    {
      rexp += 1; 
      rfrac = rfrac >> 1;
     }
    rsign = xsign; // The sign won't change from addition of like-signed opperands
  }
  
  // Normallize rfrac
  while ((rfrac & FRAC_MASK) > 0 && (rfrac & MANT_MASK) == 0) // While the MSBit is 0
  {
    rfrac = rfrac << 1;
    rexp = rexp + 1;
  }   
  rfrac = rfrac & FRAC_MASK; // Drop the implicit 1

  dandi.u =  ((rsign << SIGN_SHIFT) & SIGN_MASK) | 
             ((rexp << EXP_SHIFT) & EXP_MASK) | 
             ((rfrac << FRAC_SHIFT) & FRAC_MASK);
  toReturn = dandi.d;
  dandi2.d = toReturn;
  printf("x:%lu (0x%016lx)\ny:%lu (0x%016lx)\nx+y:%lu (0x%016lx)\n",xfrac,xfrac,yfrac,yfrac,rfrac,rfrac);
  printf("The double value %f (bit pattern 0x%02x%02x%02x%02x%02x%02x%02x%02x) represents the following:\n",
          dandi2.d, dandi2.c[0], dandi2.c[1], dandi2.c[2], dandi2.c[3], 
          dandi2.c[4], dandi2.c[5], dandi2.c[6], dandi2.c[7]); 
  printf("sign: %d (0x%lx)\texponent: %lu (0x%lx) \tmantissa: %f (0x%lx)\n", 
          rsign, ((dandi2.u & SIGN_MASK) >> SIGN_SHIFT), 
          (rexp-EXP_BIAS), ((dandi2.u & EXP_MASK) >> EXP_SHIFT), 
          ((double)rfrac/pow(2.0,54.0)), ((dandi2.u & FRAC_MASK) >> FRAC_SHIFT));
  
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
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = 7.0;
  y = 1.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = 7.0;
  y = 23.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = 7.0;
  y = 0.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = -7.0;
  y = -3.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = 7.0;
  y = -3.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = -7.0;
  y = 3.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = 9.0;
  y = -1.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
  x = 3.0;
  y = -7.0;
  printf("fadd(%f,%f) = %f\n\n",x,y,fadd(x,y));
}
