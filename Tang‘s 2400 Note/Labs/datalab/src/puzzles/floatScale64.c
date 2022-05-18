#ifdef PROTOTYPE
unsigned floatScale64(unsigned);
unsigned test_floatScale64(unsigned);
#endif
#ifdef DECL
 {"floatScale64", (funct_t) floatScale64, (funct_t) test_floatScale64, 1,
    "$", 35, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatScale64 - Return bit-level equivalent of expression 64*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 35
 *   Rating: 4
 */
unsigned floatScale64(unsigned uf) {
#ifdef FIX
  unsigned sign = uf>>31;
  unsigned exp = uf>>23 & 0xFF;
  unsigned frac = uf & 0x7FFFFF;
  if (exp == 0xFF) {
      /* Infinity or NAN.  No change required */
  } else if (exp == 0) {
      /* Initially denormalized. */
      frac = 64*frac;
      if (frac > 0x7FFFFF) {
	  /* Result will be normalized, but no rounding needed */
	  exp = 1;
	  /* Shift right until MSB in bit position 23 */
	  while (frac & ~0xFFFFFF) {
	      frac >>= 1;
	      exp++;
	  }
	  frac &= 0x7FFFFF; /* Chop off leading bit */
      }
  } else {
      /* Initially normalized.  Add leading 1 and scale by 64 */
      frac = 64 * ((1<<23) + frac);
      /* Shift right until MSB in bit position 23 */
      while (frac & ~0xFFFFFF) {
	  frac >>= 1;
	  exp++;
      }
      frac &= 0x7FFFFF; /* Chop off leading bit */
      if (exp > 0xFE) {
	  /* Overflowed to infinity */
	  frac = 0;
	  exp = 0xFF;
      }
  }
  return (sign << 31) | (exp << 23) | frac;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_floatScale64(unsigned uf) {
  float f = u2f(uf);
  float tenf = 64*f;
  if (isnan(f))
    return uf;
  else
    return f2u(tenf);
}
#endif
