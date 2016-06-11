/* printf.c */

#include <arg.h>
#include <math.h>

#include <framebuffer.h>

#include <printf.h>

int kprintf(const char* fmt, ...) {
	va_list ap;
	int ret = 0;

	va_start(ap, fmt);
	ret = kvprintf(fmt, ap);
	va_end(ap);

	return ret;
}

int kvprintf(const char* fmt, va_list ap) {
	char* s = NULL;
	char c;
	int d = 0, dd = 0, i = 0;
	int ret = 0;

	for (; *fmt != '\0'; s++) {
		if (*fmt != '%') {
			ret++;
			fb_putchar(*fmt);
			continue;
		}
		fmt++;
		switch (*fmt) {
			case 's':
				s = va_arg(ap, char*);
				if (s == NULL) {
					s = "(null)";
				}
				while (*s != '\0') {
					fb_putchar(*s);
					s++;
					ret++;
				}
				break;
			case 'c':
				c = va_arg(ap, char);
				fb_putchar(c);
				ret++;
				break;
			case 'd':
				d = va_arg(ap, int);
				if (d < 0) {
					fb_putchar('-');
					ret++;
					d *= -1;
				}
				for (i = 10; i >= 0; i--) {
					dd = (d / powint(10, i) % 10);
					if (dd != 0) {
						i--;
						break;
					}
				}
				fb_putchar(48 + dd);
				for (; i >= 0; i--) {
					dd = (d / powint(10, i) % 10);
					fb_putchar(48 + dd);
				}
			case '%':
				fb_putchar('%');
				ret++;
				break;
			default:
				fb_putchar(*fmt);
				ret++;
				break;
		}
	}

	return ret;
}
