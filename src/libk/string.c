/* strlen.c */

#include <defs.h>
#include <ints.h>

#include <string.h>

void* memset(void* s, int c, size_t n) {
	size_t i;
	u8* p = s;

	for (i = 0; i < n; i++) {
		p[i] = c;
	}

	return s;
}

size_t strlen(const char* str) {
	size_t ret;

	for (ret = 0; ; ret++) {
		if (str[ret] == '\0') {
			break;
		}
	}

	return ret;
}
