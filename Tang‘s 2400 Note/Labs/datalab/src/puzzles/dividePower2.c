#ifdef PROTOTYPE
int dividePower2(int, int);
int test_dividePower2(int, int);
#endif
#ifdef DECL
 {"dividePower2", (funct_t) dividePower2, (funct_t) test_dividePower2, 2,
    "! ~ & ^ | + << >>", 15, 2,
  {{TMin, TMax},{0,30},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * dividePower2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: dividePower2(15,1) = 7, dividePower2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int dividePower2(int x, int n) {
#ifdef FIX
    /* Handle rounding by generating bias:
       0 when x >= 0
       2^n-1 when x < 0
    */
    int mask = (1 << n) + ~0;
    int bias = (x >> 31) & mask;
    return (x+bias) >> n;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_dividePower2(int x, int n)
{
    int p2n = 1<<n;
    return x/p2n;
		
}
#endif
