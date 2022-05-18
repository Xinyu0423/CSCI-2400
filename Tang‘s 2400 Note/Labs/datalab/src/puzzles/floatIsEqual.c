#ifdef PROTOTYPE
int floatIsEqual(unsigned, unsigned);
int test_floatIsEqual(unsigned, unsigned);
#endif
#ifdef DECL
 {"floatIsEqual", (funct_t) floatIsEqual, (funct_t) test_floatIsEqual, 2,
    "$", 25, 2,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
#ifdef FIX
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
	return  1;
    return uf == ug;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_floatIsEqual(unsigned uf, unsigned ug) {
    float f = u2f(uf);
    float g = u2f(ug);
    return f == g;
}
#endif
