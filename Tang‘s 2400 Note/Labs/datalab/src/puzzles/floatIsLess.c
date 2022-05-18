#ifdef PROTOTYPE
int floatIsLess(unsigned, unsigned);
int test_floatIsLess(unsigned, unsigned);
#endif
#ifdef DECL
 {"floatIsLess", (funct_t) floatIsLess, (funct_t) test_floatIsLess, 2,
    "$", 30, 3,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
int floatIsLess(unsigned uf, unsigned ug) {
#ifdef FIX
    unsigned signf = uf >> 31;
    unsigned expf = (uf >> 23) & 0xFF;
    unsigned fracf = uf & 0x7FFFFF;
    int nanf = expf == 0xFF && fracf != 0;
    int zerof = expf == 0 && fracf == 0;

    unsigned signg = ug >> 31;
    unsigned expg = (ug >> 23) & 0xFF;
    unsigned fracg = ug & 0x7FFFFF;
    int nang = expg == 0xFF && fracg != 0;
    int zerog = expg == 0 && fracg == 0;

    if (nanf || nang)
	return 0;
    if (zerof && zerog)
	return  0;
    else if (signf) {
	/* f < 0 */
	if (signg)
	    return ug < uf;
	else
	    return 1;
    } else {
	if (signg)
	    return 0;
	else
	    return uf < ug;
    }
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_floatIsLess(unsigned uf, unsigned ug) {
    float f = u2f(uf);
    float g = u2f(ug);
    return f < g;
}
#endif
