#ifdef PROTOTYPE
int distinctNegation(int);
int test_distinctNegation(int);
#endif
#ifdef DECL
 {"distinctNegation", (funct_t) distinctNegation, (funct_t) test_distinctNegation, 1, "! ~ & ^ | +", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * distinctNegation - returns 1 if x != -x.
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 5
 *   Rating: 2
 */
int distinctNegation(int x) {
#ifdef FIX
    /* x must be either 0 or Tmin */
  return !!(x+x);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_distinctNegation(int x) {
    return x != -x;
}
#endif
