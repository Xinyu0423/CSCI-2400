#ifdef PROTOTYPE
unsigned floatScale4(unsigned);
unsigned test_floatScale4(unsigned);
#endif
#ifdef DECL
 {"floatScale4", (funct_t) floatScale4, (funct_t) test_floatScale4, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatScale4 - Return bit-level equivalent of expression 4*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale4(unsigned uf) {
#ifdef FIX
    unsigned sign = uf>>31;
    unsigned exp = uf>>23 & 0xFF;
    unsigned frac = uf & 0x7FFFFF;
    if (exp == 0) {
	/* Denormalized.  Must scale fraction */
	frac = 4*frac;
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
    } else if (exp < 0xFF) {
	/* Normalized.  Increase exponent */
	exp += 2;
	if (exp >= 0xFF) {
	    /* Infinity */
	    exp = 0xFF;
	    frac = 0;
	}
    }
    /* Infinity and NaN do not require any changes */
    return (sign << 31) | (exp << 23) | frac;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_floatScale4(unsigned uf) {
  float f = u2f(uf);
  float tf = 4*f;
  if (isnan(f))
    return uf;
  else
    return f2u(tf);
}
#endif
