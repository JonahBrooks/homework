#define SIZE 10

int nondet_int();

int binsearch (int a[], int t, int s) {
  int l, r, m;
  l = 0;
  r = s-1;
  while (l <= r) {
    m = ((r-l)/2)+l;
    if (a[m] == t)
      return m;
    else if (a[m] > t) {
      r = m-1;
    } else {
      l = m+1;
    }
  }
  return -1;
}

int main () {
  int a[SIZE];
  int i, v;
  int t = nondet_int();
  int in = 0;

  for (i = 0; i < SIZE; i++) {
    a[i] = nondet_int();
    if (i > 0) {
      __CPROVER_assume(a[i] > a[i-1]);
    }
    if (a[i] == t)
      in = 1;
  }
  
  v = binsearch(a, t, SIZE);

  assert ((!in && (v == -1)) || (a[v] == t));
}
