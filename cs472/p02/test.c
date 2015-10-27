#include "frexp.h"
#include "extraction.h"
#include "fops.h"
#include "hw2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv)
{

  double x;
  double f;
  double fr;
  int exp;
  int expr;

  printf("Testing myfrexp() and comparing to values extracted from the double:\n");
  printf("  Special cases of 0, NAN, INF, and -INF:\n");
  x = 0.0;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("    myfrexp vs frexp:\n  ");
  printf("    x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = NAN;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("    myfrexp vs frexp:\n  ");
  printf("    x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = INFINITY;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("    myfrexp vs frexp:\n  ");
  printf("    x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = -INFINITY;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("    myfrexp vs frexp:\n  ");
  printf("    x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  printf("\n  Testing myfrexp with 10 random doubles:\n");
  srand48((unsigned)time(NULL));
  int i;
  for (i = 0; i < 10; i++)
  {
    x = (double)drand48()*RAND_MAX;
    fr = frexp(x,&expr);
    f = myfrexp(x,&exp);
    extraction(x);
    printf("    myfrexp vs frexp:\n  ");
    printf("    x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);
  } 
  
  // Call fops for the rest of the output
  fops();

  return 0;
}
