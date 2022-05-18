#ifdef PROTOTYPE
int specialBits();
int test_specialBits();
#endif
#ifdef DECL
 {"specialBits", (funct_t) specialBits, (funct_t) test_specialBits, 0,
    "! ~ & ^ | + << >>", 3, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * specialBits - return bit pattern 0xffca3fff
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 3
 *   Rating: 1
 */
int specialBits(void) {
#ifdef FIX
    int mask = 0xd7 << 14;
    int neg1 = ~0;
    return neg1 ^ mask;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_specialBits(void) {
    return 0xffca3fff;
}
#endif
