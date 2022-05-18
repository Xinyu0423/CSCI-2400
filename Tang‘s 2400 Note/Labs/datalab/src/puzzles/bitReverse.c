#ifdef PROTOTYPE
int bitReverse(int);
int test_bitReverse(int);
#endif
#ifdef DECL
{"bitReverse", (funct_t) bitReverse, (funct_t) test_bitReverse, 1, "! ~ & ^ | + << >>", 45, 4,
	{{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * bitReverse - Reverse bits in a 32-bit word
 *   Examples: bitReverse(0x80000002) = 0x40000001
 *             bitReverse(0x89ABCDEF) = 0xF7D3D591
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 45
 *   Rating: 4
 */
int bitReverse(int x) {
#ifdef FIX  
    /* 
       Reverse adjacent blocks of bits,
       starting with block size = 16
       down to block size = 1
    */
    int m16, nlo16, nhi16;
    int m8, nlo8, nhi8;
    int m4a, m4, nlo4, nhi4;
    int m2a, m2, nlo2, nhi2;
    int m1a, m1, nlo1, nhi1;
    /* Swap lower & upper 16 bytes */
    m16 = (0xFF << 8) | 0xFF;
    nlo16 = (x >> 16) & m16;
    nhi16 = x << 16;
    x = nhi16 | nlo16;
    /* Swap groups of 8 bits */
    m8 = (0xFF << 16) | 0xFF;
    nlo8 = (x >> 8) & m8;
    nhi8 = (x & m8) << 8;
    x = nhi8 | nlo8;
    /* Swap groups of 4 bits */
    m4a = (0xF << 8) | 0xF;
    m4 = (m4a << 16) | m4a;
    nlo4 = (x >> 4) & m4;
    nhi4 = (x & m4) << 4;
    x = nhi4 | nlo4;
    /* Swap groups of 2 bits */
    m2a = (0x33 << 8) | 0x33;
    m2 = (m2a << 16) | m2a;
    nlo2 = (x >> 2) & m2;
    nhi2 = (x & m2) << 2;
    x = nhi2 | nlo2;
    /* Swap adjacent bits */
    m1a = (0x55 << 8) | 0x55;
    m1 = (m1a << 16) | m1a;
    nlo1 = (x >> 1) & m1;
    nhi1 = (x & m1) << 1;
    x = nhi1 | nlo1;
    return x;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_bitReverse(int x) {
    int result = 0;
    int i;
    for (i = 0; i < 32; i++) {
	int bit = (x >> i) & 0x1;
	int pos = 31-i;
	result |= bit << pos;
    }
    return result;
}
#endif
