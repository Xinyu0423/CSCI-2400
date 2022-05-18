#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

/* Fix any NaN to be NaN value 0x7FC00000 */
float fix_nan(float f)
{
  if (isnanf(f))
    return u2f(0x7FC0000);
  else
    return f;
}

int floatIsEqual(unsigned uf, unsigned ug) {
    unsigned expf = (uf >> 23) & 0xFF;
    unsigned fracf = uf & 0x7FFFFF;
    int nanf = expf == 0xFF && fracf != 0;
    int zerof = expf == 0 && fracf == 0;
    unsigned expg = (ug >> 23) & 0xFF;
    unsigned fracg = ug & 0x7FFFFF;
    int nang = expg == 0xFF && fracg != 0;
    int zerog = expg == 0 && fracg == 0;
    if (nanf || nang)
     return 0;
    if (zerof && zerog)
 return 1;
    return uf == ug;
}


int test_floatIsEqual(unsigned uf, unsigned ug) {
    float f = u2f(uf);
    float g = u2f(ug);
    return f == g;
}


int main()
{
    int val1 = floatIsEqual(4294311499, 4294311499);
    int val2 = test_floatIsEqual(4294311499, 4294311499);
    printf("floatIsEqual(4294311499, 4294311499) --> %d [0x%x]\n", val1, val1);
    printf("test_floatIsEqual(4294311499, 4294311499) --> %d [0x%x]\n", val2, val2);
    if (val1 == val2) {
	printf(".. False negative\n");
    } else
	printf(".. A genuine counterexample\n");
}
