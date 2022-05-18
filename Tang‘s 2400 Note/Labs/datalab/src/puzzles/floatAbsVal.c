#ifdef PROTOTYPE
unsigned floatAbsVal(unsigned);
unsigned test_floatAbsVal(unsigned);
#endif
#ifdef DECL
 {"floatAbsVal", (funct_t) floatAbsVal, (funct_t) test_floatAbsVal, 1,
    "$", 10, 2,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {
#ifdef FIX
  unsigned mask = 1 << 31;
  unsigned abs = uf & ~mask;
  if (abs > 0x7F800000)
    return uf;
  return abs;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_floatAbsVal(unsigned uf) {
  float f = u2f(uf);
  unsigned unf = f2u(-f);
  if (isnan(f))
    return uf;
  /* An unfortunate hack to get around a limitation of the BDD Checker */
  if ((int) uf < 0)
      return unf;
  else
      return uf;
}
#endif
