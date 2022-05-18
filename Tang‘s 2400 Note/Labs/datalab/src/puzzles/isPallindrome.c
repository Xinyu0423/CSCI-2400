#ifdef PROTOTYPE
int isPallindrome(int);
int test_isPallindrome(int);
#endif
#ifdef DECL
{"isPallindrome", (funct_t) isPallindrome, (funct_t) test_isPallindrome, 1, "! ~ & ^ | + << >>", 40, 4,
	{{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * isPallindrome - Return 1 if bit pattern in x is equal to its mirror image
 *   Example: isPallindrome(0x01234567E6AC2480) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 45
 *   Rating: 4
 */
int isPallindrome(int x) {
#ifdef FIX  
    /* 
       Reverse lower 16 bits of x
       starting with block size = 8
       down to block size = 1
    */
    int m16, xlo16, xhi16;
    int m8, nlo8, nhi8;
    int m4, nlo4, nhi4;
    int m2, nlo2, nhi2;
    int m1, nlo1, nhi1;
    m16 = (0xFF << 8) | 0xFF;
    xlo16 = x & m16;
    /* Swap groups of 8 bits */
    m8 = 0xFF;
    nlo8 = (xlo16 >> 8) & m8;
    nhi8 = (xlo16 & m8) << 8;
    xlo16 = nhi8 | nlo8;
    /* Swap groups of 4 bits */
    m4 = (0xF << 8) | 0xF;
    nlo4 = (xlo16 >> 4) & m4;
    nhi4 = (xlo16 & m4) << 4;
    xlo16 = nhi4 | nlo4;
    /* Swap groups of 2 bits */
    m2 = (0x33 << 8) | 0x33;
    nlo2 = (xlo16 >> 2) & m2;
    nhi2 = (xlo16 & m2) << 2;
    xlo16 = nhi2 | nlo2;
    /* Swap adjacent bits */
    m1 = (0x55 << 8) | 0x55;
    nlo1 = (xlo16 >> 1) & m1;
    nhi1 = (xlo16 & m1) << 1;
    xlo16 = nhi1 | nlo1;
    /* Compare to upper 16 bits */
    xhi16 = (x >> 16) & m16;
    return !(xlo16 ^ xhi16);
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_isPallindrome(int x) {
    int result = 1;
    int i;
    int mask = 0xFFFF;
    int xlo = x & mask;
    int xhi = (x >> 16) & mask;
    for (i = 0; i < 16; i++) {
	int flipi = 15-i;
	int bhigh = (xhi >> i) & 0x1;
	int blow  = (xlo>> flipi) & 0x1;
	result = result && (bhigh == blow);
    }
    return result;
}
#endif
