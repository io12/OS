/* kprintf.c */

#include <arg.h>
#include <defs.h>

#include <framebuffer.h>
#include <serial.h>

#include <kprintf.h>

#define INT_BUF_SIZE 10
#define PUTCHAR(X)   (pl == PL_FRAMEBUFFER ? fb_putchar(X) : serial_putchar(X))
#define PUTS(X)      (pl == PL_FRAMEBUFFER ? fb_puts(X)    : serial_puts(X))

void dec_to_str(char* buf, int num) {
	char backwards_buf[INT_BUF_SIZE] = {0};
	int i = 0;
	int j = 0;

	// print '-' and make num positive if negative
	if (num < 0) {
		backwards_buf[i] = '-';
		i++;
		num *= -1;
	}
	// store digits backwards in buffer
	while (num != 0 && i < INT_BUF_SIZE) {
		backwards_buf[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	// reverse backwards_buf to get buf
	for (i = INT_BUF_SIZE - 1; i >= 0; i--) {
		if (backwards_buf[i] != '\0') {
			buf[j] = backwards_buf[i];
			j++;
		}
	}
	buf[j] = '\0';
}

void kprint_dec(PrintLocation pl, int num) {
	char buf[INT_BUF_SIZE] = {0};
	dec_to_str(buf, num);
	PUTS(buf);
}

void kprintf(PrintLocation pl, const char* fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	kvprintf(pl, fmt, ap);
	va_end(ap);
}

void kvprintf(PrintLocation pl, const char* fmt, va_list ap) {
	char* s = NULL;

	for (; *fmt != '\0'; fmt++) {
		if (*fmt != '%') {
			PUTCHAR(*fmt);
			continue;
		}
		fmt++;
		switch (*fmt) {
			case 's':
				s = va_arg(ap, char*);
				if (s == NULL) {
					PUTS("(null)");
				}
				else {
					PUTS(s);
				}
				break;
			case 'c':
				PUTCHAR(va_arg(ap, int));
				break;
			case 'd':
				kprint_dec(pl, va_arg(ap, int));
				break;
			case '%':
				PUTCHAR('%');
				break;
			case '\0':
				return;
			default:
				PUTCHAR(*fmt);
		}
	}
}
