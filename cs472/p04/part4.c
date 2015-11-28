#include <stdio.h>

int main(int argc, char **argv)
{

  union byte_check {

    char bytes[2];
    short srt;

  }check;

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

  check.srt = 0x0102;
  if(check.bytes[0] != 0x01)
  {
    tmp = p_val[1];
    p_val[1] = p_val[0];
    p_val[0] = tmp;
  }
  // else: it's already in big endian

  printf("%x\n", val);
  
  return 0;
}


