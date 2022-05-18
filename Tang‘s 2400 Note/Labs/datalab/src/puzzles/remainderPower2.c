#ifdef PROTOTYPE
int remainderPower2(int, int);
int test_remainderPower2(int, int);
#endif
#ifdef DECL
 {"remainderPower2", (funct_t) remainderPower2, (funct_t) test_remainderPower2, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{0,30},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * remainderPower2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: remainderPower2(15,2) = 3, remainderPower2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int remainderPower2(int x, int n) {
#ifdef FIX
    /* First compute x / (1<<n) */
    int mask = (1 << n) + ~0;
    int bias = (x >> 31) & mask;
    int quo  = (x+bias) >> n;
    /* Now multiply by (1<<n) and subtract off */
    int factor = ~(quo << n) + 1;
    return factor + x;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_remainderPower2(int x, int n)
{
    int p2n = 1<<n;
    return x%p2n;
}
#endif
