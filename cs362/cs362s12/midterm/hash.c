#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define abs(x) (x < 0 ? -x : x)

#define mix(a,b,c) \
{ \
  a -= c;  a ^= rot(c, 4);  c += b; \
  b -= a;  b ^= rot(a, 6);  a += c; \
  c -= b;  c ^= rot(b, 8);  b += a; \
  a -= c;  a ^= rot(c,16);  c += b; \
  b -= a;  b ^= rot(a,19);  a += c; \
  c -= b;  c ^= rot(b, 4);  b += a; \
}

#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c,4);  \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

unsigned int nondet_uint();

unsigned short hash(unsigned int x, unsigned int s)
{
  char *k = (char*)&x;

  unsigned int length = 1;

  unsigned short a,b,c;

  a = b = c = 0xdeadbeef + (4<<2) + s;

  a += k[0];
  b += k[1];
  c += k[2];
  mix(a,b,c);
  a+=k[0];
  final(a,b,c);

  printf ("CE:  c = %u\n", c);
  a = (x % 22); // Make sure not equal within 20, with a little room to grow in!
  printf ("CE:  a = %u\n", a);
  c *= 100;
  printf ("CE:  c = %u\n", c);
  c += a;
  printf ("CE:  c = %u\n", c);

  return c;
}

int main () {
  unsigned int x1 = nondet_uint();
  unsigned int x2 = nondet_uint;
  unsigned int s = nondet_uint();

  printf ("CE:  x1=%u, x2=%u, s=%u\n", x1, x2, s);
  
  __CPROVER_assume(abs(x1-x2) <= 20);
  __CPROVER_assume(x1 != x2);

  unsigned short hx1 = hash(x1, s);
  printf ("CE:  hash(x1,s) = %u\n", hx1);

  unsigned short hx2 = hash(x2, s);
  printf ("CE:  hash(x2,s) = %u\n", hx2);

  assert (hx1 != hx2);
}
