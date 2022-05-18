#ifdef PROTOTYPE
int bitMatch(int, int);
int test_bitMatch(int, int);
#endif
#ifdef DECL
 {"bitMatch", (funct_t) bitMatch, (funct_t) test_bitMatch, 2, "& ~ |", 14, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and & 
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
#ifdef FIX
  int x_and_y = x&y;
  int nx_and_ny = (~x & ~y);
  return x_and_y | nx_and_ny;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitMatch(int x, int y)
{
    int i;
    int result = 0;
    for (i = 0; i < 32; i++) {
	int mask = 1 << i;
	int bit = (x & mask) == (y & mask);
	result |=  bit << i;
    }
    return result;
}
#endif
