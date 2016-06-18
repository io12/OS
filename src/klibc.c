/* klibc.c */

#include <ints.h>

#include <klibc.h>

void* memset(void* s, int c, size_t n) {
	size_t i;
	volatile u8* p = s;

	for (i = 0; i < n; i++) {
		p[i] = c;
	}

	return (void*) p;
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
