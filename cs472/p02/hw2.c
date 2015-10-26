#include <string.h>

inline void flip(void *x)
{
  char *c = (char *) x;
  char t[8];
  t[0] = c[7];
  t[1] = c[6];
  t[2] = c[5];
  t[3] = c[4];
  t[4] = c[3];
  t[5] = c[2];
  t[6] = c[1];
  t[7] = c[0];
  memcpy(x,t,8);
}
