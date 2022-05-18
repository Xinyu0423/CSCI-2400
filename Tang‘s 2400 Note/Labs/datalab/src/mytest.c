#include <stdio.h>
#include <stdlib.h>

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

void compare(float f, float g) {
    printf("f = %f, g = %f\n", f, g);
    printf("f == g ? %s\n", f == g ? "Yes" : "No");
    printf("f != g ? %s\n", f != g ? "Yes" : "No");
    printf("f < g ? %s\n", f < g ? "Yes" : "No");
    printf("f <= g ? %s\n", f <= g ? "Yes" : "No");
    printf("f > g ? %s\n", f > g ? "Yes" : "No");
    printf("f >= g ? %s\n", f >= g ? "Yes" : "No");
}

int main(int argc, char *argv[]) {
    unsigned uf = 0;
    unsigned ug = 0;
    if (argc >= 2)
	uf = strtoul(argv[1], NULL, 0);
    if (argc >= 3)
	ug = strtoul(argv[2], NULL, 0);
    float f = u2f(uf);
    float g = u2f(ug);
    compare(f, g);
    return 0;
}
