#include "frexp.h"
#include "extraction.h"
#include "fops.h"
#include "hw2.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv)
{

  double x;
  double f;
  double fr;
  int exp;
  int expr;

  x = 0.0;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = NAN;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = INFINITY;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = -INFINITY;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = M_PI;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = -M_PI;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = 1;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = 1024;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = 69105.4022;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  x = 3;
  fr = frexp(x,&expr);
  f = myfrexp(x,&exp);
  extraction(x);
  printf("myfrexp vs frexp:\n  ");
  printf("x:%f\tMyFraction:%f\tMyExp:%d\tTrueFraction:%f\tTrueExp:%d\n",x,f,exp,fr,expr);

  fops();

  return 0;
}
