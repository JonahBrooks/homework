int main () {
  int i = 0;
  int j; /* CRITICAL */
  int k;
  j = 4; /* CRITICAL */
  k = 32;
  int *p = &i;
  if (j == 4) { /* CRITICAL */
    p = 0; /* CRITICAL */
    k = 16;
    j = 12;
  }
  printf ("OK: *p = %d\n", *p); /* CRITICAL */
}
