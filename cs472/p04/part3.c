#include <stdio.h>

int main(int argc, char **argv)
{
  short val;
  char *p_val;
  p_val = (char *) &val;
  char tmp;
  /*
 *    The following two lines assume big-endian
 *        Architecture to initialize the variable Val
 *            to 0x1234.
 *              */
  p_val[0] = 0x12;
  p_val[1] = 0x34;

#if __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__
  tmp = p_val[1];
  p_val[1] = p_val[0];
  p_val[0] = tmp;
#endif
// else: Don't flip anything, since it;s already in big endian

  printf("%x\n", val);

  
  return 0;
}


