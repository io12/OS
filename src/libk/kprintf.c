/* kprintf.c */

#include <arg.h>

#include <framebuffer.h>

#include <kprintf.h>

#define INT_BUF_SIZE 10

void kprint_dec(int num) {
	char buf[INT_BUF_SIZE] = {'\0'};
	int i;

	// print '-' and make num positive if negative
	if (num < 0) {
		fb_putchar('-');
		num *= -1;
	}
	// store digits backwards in buffer
	for (i = 0; num != 0 && i < INT_BUF_SIZE; i++) {
		buf[i] = (num % 10) + '0';
		num /= 10;
	}

	// print buffer backwards
	for (i = INT_BUF_SIZE - 1; i >= 0; i--) {
		if (buf[i] != '\0') {
			fb_putchar(buf[i]);
		}
	}
}

void kprintf(const char* fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	kvprintf(fmt, ap);
	va_end(ap);
}

void kvprintf(const char* fmt, va_list ap) {
	char* s = NULL;

	for (; *fmt != '\0'; fmt++) {
		if (*fmt != '%') {
			fb_putchar(*fmt);
			continue;
		}
		fmt++;
		switch (*fmt) {
			case 's':
				s = va_arg(ap, char*);
				if (s == NULL) {
					fb_puts("(null)");
				}
				else {
					fb_puts(s);
				}
				break;
			case 'c':
				fb_putchar(va_arg(ap, int));
				break;
			case 'd':
				kprint_dec(va_arg(ap, int));
				break;
			case '%':
				fb_putchar('%');
				break;
			case '\0':
				return;
			default:
				fb_putchar(*fmt);
		}
	}
}
