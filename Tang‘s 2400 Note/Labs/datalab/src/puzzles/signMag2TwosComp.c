#ifdef PROTOTYPE
int signMag2TwosComp(int);
int test_signMag2TwosComp(int);
#endif
#ifdef DECL
 {"signMag2TwosComp", (funct_t) signMag2TwosComp, (funct_t) test_signMag2TwosComp, 1, "! ~ & ^ | + << >>", 15, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * signMag2TwosComp - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: signMag2TwosComp(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int signMag2TwosComp(int x) {
#ifdef FIX
  int mask = x>>31;
  int mag = x & ~(1<<31);
  return (mag ^ mask) + ~mask + 1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_signMag2TwosComp(int x) {
  int sign = x < 0;
  int mag  = x & 0x7FFFFFFF;
  return sign ? -mag : mag;
}
#endif
