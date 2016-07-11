/* kprintf.c */

#include <framebuffer.h>
#include <ints.h>
#include <klibc.h>
#include <system.h>

#include <kprintf.h>

#define DEC_BUF_SIZE 11
#define HEX_BUF_SIZE 10
#define PUTCHAR(X)   \
	do { \
		if (pl == PL_ALL || pl == PL_FRAMEBUFFER) { \
			fb_putchar(X); \
		} \
		if (pl == PL_ALL || pl == PL_SERIAL) { \
			serial_putchar(X); \
		} \
	} while (0)
#define PUTS(X)   \
	do { \
		if (pl == PL_ALL || pl == PL_FRAMEBUFFER) { \
			fb_puts(X); \
		} \
		if (pl == PL_ALL || pl == PL_SERIAL) { \
			serial_puts(X); \
		} \
	} while (0)

char hex_digits[] = "0123456789ABCDEF";

void udec_to_str(char* buf, u32 num) {
	char backwards_buf[DEC_BUF_SIZE] = {'\0'};
	i16 i = 0;
	i16 j = 0;

	// if number is zero, return early
	if (num == 0) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}
	// store digits backwards in buffer
	while (num != 0 && i < DEC_BUF_SIZE) {
		backwards_buf[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	// reverse backwards_buf to get buf
	for (i = DEC_BUF_SIZE - 1; i >= 0; i--) {
		if (backwards_buf[i] != '\0') {
			buf[j] = backwards_buf[i];
			j++;
		}
	}
	buf[j] = '\0';
}

void uhex_to_str(char* buf, u32 num) {
	char backwards_buf[HEX_BUF_SIZE] = {'\0'};
	i16 i = 0;
	i16 j = 0;
	

	// if number is zero, return early
	if (num == 0) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}
	// store digits backwards in buffer
	while (num != 0 && i < HEX_BUF_SIZE) {
		backwards_buf[i] = hex_digits[num % 0x10];
		num /= 0x10;
		i++;
	}
	// reverse backwards_buf to get buf
	for (i = HEX_BUF_SIZE - 1; i >= 0; i--) {
		if (backwards_buf[i] != '\0') {
			buf[j] = backwards_buf[i];
			j++;
		}
	}
	buf[j] = '\0';
}

void kprint_udec(PrintLocation pl, u32 num) {
	char buf[DEC_BUF_SIZE];

	udec_to_str(buf, num);
	PUTS(buf);
}

void kprint_uhex(PrintLocation pl, u32 num) {
	char buf[HEX_BUF_SIZE];

	uhex_to_str(buf, num);
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
			case 'u':
				kprint_udec(pl, va_arg(ap, u32));
				break;
			case 'X':
				kprint_uhex(pl, va_arg(ap, u32));
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
