#include <ints.h>
#include <liballoc.h>

#include <klibc.h>

// stdio.h

int sprintf(char* buf, const char* fmt, ...) {
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsprintf(buf, fmt, ap);
	va_end(ap);

	return ret;
}

int vsprintf(char* buf, const char* fmt, va_list ap) {
	char* str;
	int ret;

	for (; *fmt != '\0'; fmt++) {
		if (*fmt != '%') {
			*buf = *fmt;
			buf++;
			continue;
		}
		fmt++;
		switch (*fmt) {
			case 's':
				str = va_arg(ap, char*);
				if (str == NULL) {
					str = "(null)"
				}
				else {
					strcpy(buf, str);
					buf += strlen(str);
				}
				break;
			case 'c':
				*buf = va_arg(ap, int);
				buf++;
				break;
			case 'u':
				buf += udec_to_str(buf, va_arg(ap, int));
				break;
			case 'X':
				buf += uhex_to_str(buf, va_arg(ap, int));
				break;
			case '%':
				*buf = '%';
				buf++;
				break;
			case '\0':
				return;
			default:
				*buf = *fmt;
				buf++;
		}
	}

	return ret;
}

// string.h

int memcmp(const void* p1, const void* p2, size_t n) {
	size_t i;
	const u8* byte_p1 = p1;
	const u8* byte_p2 = p2;

	for (i = 0; i < n; i++) {
		if (*byte_p1 != *byte_p2) {
			return *byte_p1 - *byte_p2;
		}
		byte_p1++;
		byte_p2++;
	}

	return 0;
}

void* memcpy(void* dest, const void* src, size_t n) {
	size_t i;
	u8* dest_ptr = dest;
	const u8* src_ptr = src;

	for (i = 0; i < n; i++) {
		dest_ptr[i] = src_ptr[i];
	}

	return dest;
}

void* memset(void* ptr, int c, size_t n) {
	size_t i;
	volatile u8* byte_ptr = ptr;

	for (i = 0; i < n; i++) {
		byte_ptr[i] = c;
	}

	return ptr;
}

char* strchr(const char* str, int c) {
	while (*str != c) {
		if (*str == '\0') {
			return NULL;
		}
		str++;
	}

	return (char*) str;
}

int strcmp(const char* s1, const char* s2) {
	while (*s1 != '\0') {
		if (*s1 > *s2 || *s2 == '\0') {
			return 1;
		}
		else if (*s1 < *s2) {
			return -1;
		}
		s1++;
		s2++;
	}
	if (*s2 != '\0') {
		return -1;
	}

	return 0;
}

char* strcpy(char* dest, const char* src) {
	return memcpy(dest, src, strlen(src) + 1);
}

char* strdup(const char* str) {
	return strcpy(malloc(strlen(str) + 1), str);
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
