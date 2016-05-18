/* strlen.c */

#include <string.h>

// Calculates the length of a string
size_t strlen(const char* str) {
	size_t ret;

	for (ret = 0; ; ret++) {
		if (str[ret] == '\0') {
			break;
		}
	}

	return ret;
}
