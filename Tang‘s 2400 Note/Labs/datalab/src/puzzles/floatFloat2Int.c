#ifdef PROTOTYPE
int floatFloat2Int(unsigned);
int test_floatFloat2Int(unsigned);
#endif
#ifdef DECL
 {"floatFloat2Int", (funct_t) floatFloat2Int, (funct_t) test_floatFloat2Int, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
#ifdef FIX
  unsigned sign = uf >> 31;
  unsigned exp = (uf >> 23) & 0xFF;
  unsigned frac = uf & 0x7FFFFF;
  /* Create normalized value with leading one inserted,
     and rest of significand in bits 8--30.
  */
  unsigned val = 0x80000000u + (frac << 8);
  if (exp < 127) {
    /* Absolute value is < 1 */
    return 0;
  }
  if (exp > 158)
    /* Overflow */
    return 0x80000000u;
  /* Shift val right */
  val = val >> (158 - exp);
  /* Check if out of range */
  if (sign) {
    /* Negative */
    return val > 0x80000000u ? 0x80000000u : -val;
  } else {
    /* Positive */
    return val > 0x7FFFFFFF ? 0x80000000u : val;
  }
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_floatFloat2Int(unsigned uf) {
  float f = u2f(uf);
  int x = (int) f;
  return x;
}
#endif
