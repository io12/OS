/* klibc.c */

#include <ints.h>

#include <klibc.h>

void* memcpy(void* dest, const void* src, size_t n) {
	size_t i;
	u8* dest_ptr = dest;
	const u8* src_ptr = src;

	for (i = 0; i < n; i++) {
		dest_ptr[i] = src_ptr[i];
	}

	return dest;
}

void* memset(void* s, int c, size_t n) {
	size_t i;
	u8* ptr = s;

	for (i = 0; i < n; i++) {
		ptr[i] = c;
	}

	return (void*) ptr;
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
