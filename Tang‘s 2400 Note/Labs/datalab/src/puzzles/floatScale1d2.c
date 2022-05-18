#ifdef PROTOTYPE
unsigned floatScale1d2(unsigned);
unsigned test_floatScale1d2(unsigned);
#endif
#ifdef DECL
 {"floatScale1d2", (funct_t) floatScale1d2, (funct_t) test_floatScale1d2, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatScale1d2 - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale1d2(unsigned uf) {
#ifdef FIX
  unsigned sign = uf>>31;
  unsigned exp = uf>>23 & 0xFF;
  unsigned frac = uf & 0x7FFFFF;
  /* Only roundup case will be when rounding to even */
  unsigned roundup = (frac & 0x3) == 3;
  if (exp == 0) {
    /* Denormalized.  Must halve fraction */
    frac = (frac >> 1) + roundup;
  } else if (exp < 0xFF) {
    /* Normalized.  Decrease exponent */
    exp--;
    if (exp == 0) {
      /* Denormalize, adding back leading one */
      frac = (frac >> 1) + roundup + 0x400000;
    }
  }
  /* NaN Infinity do not require any changes */
  return (sign << 31) | (exp << 23) | frac;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_floatScale1d2(unsigned uf) {
  float f = u2f(uf);
  float hf = 0.5*f;
  if (isnan(f))
    return uf;
  else
    return f2u(hf);
}
#endif
