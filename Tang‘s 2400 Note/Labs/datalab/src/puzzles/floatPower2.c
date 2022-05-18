
#ifdef PROTOTYPE
unsigned floatPower2(int);
unsigned test_floatPower2(int);
#endif
#ifdef DECL
 {"floatPower2", (funct_t) floatPower2, (funct_t) test_floatPower2, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
#ifdef FIX
    unsigned exp, frac; 
    unsigned u; 

    if (x < -149) { 
	/* Too small.  Return 0.0 */ 
	exp  = 0; 
	frac = 0; 
    } else if (x < -126) { 
	/* Denormalized result */ 
	exp  = 0; 
	frac = 1 << (x + 149); 
    } else if (x < 128) { 
	/* Normalized result. */ 
	exp  = x + 127; 
	frac = 0; 
    } else { 
	/* Too big.  Return +oo */ 
	exp  = 255; 
	frac = 0; 
    } 
    u = exp << 23 | frac; 
    return u; 
#else
    return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_floatPower2(int x) {
  float result = 1.0;
  float p2 = 2.0;
  int recip = (x < 0);

  /* treat tmin specially */
  if ((unsigned)x == 0x80000000) {
      return 0;
  }

  if (recip) {
    x = -x;
    p2 = 0.5;
  }
  while (x > 0) {
    if (x & 0x1)
      result = result * p2;
    p2 = p2 * p2;
    x >>= 1;
  }
  return f2u(result);
}
#endif
