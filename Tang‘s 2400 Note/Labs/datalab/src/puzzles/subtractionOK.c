#ifdef PROTOTYPE
int subtractionOK(int, int);
int test_subtractionOK(int, int);
#endif
#ifdef DECL
 {"subtractionOK", (funct_t) subtractionOK, (funct_t) test_subtractionOK, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * subtractionOK - Determine if can compute x-y without overflow
 *   Example: subtractionOK(0x80000000,0x80000000) = 1,
 *            subtractionOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subtractionOK(int x, int y) {
#ifdef FIX
  int diff = x+~y+1;
  int x_neg = x>>31;
  int y_neg = y>>31;
  int d_neg = diff>>31;
  /* Overflow when x and y have opposite sign, and d different from x */
  return !(~(x_neg ^ ~y_neg) & (x_neg ^ d_neg));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_subtractionOK(int x, int y)
{
  long long ldiff = (long long) x - y;
  return ldiff == (int) ldiff;
}
#endif
