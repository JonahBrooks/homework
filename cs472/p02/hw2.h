#define SIGN_MASK 0x8000000000000000
#define SIGN_SHIFT 63
#define EXP_MASK  0x7ff0000000000000
#define EXP_SHIFT  52
#define EXP_BIAS 1023
#define FRAC_MASK 0x000fffffffffffff
#define FRAC_SHIFT 0
#define MANT_MASK 0x0010000000000000

union dandi_union
{
  double d;
  long unsigned int u;
  long int i;
  unsigned char c[8];
} dandi, dandi2;

inline void flip(void *x);
