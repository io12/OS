/* math.c */

#include <math.h>

int powint(int base, int exp) {
	int i;
	int ret = 1;

	for (i = 0; i < exp; i++) {
		ret *= base;
	}

	return ret;
}
