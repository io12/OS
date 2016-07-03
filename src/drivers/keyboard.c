/* keyboard.c */

#include <framebuffer.h>
#include <interrupt_handler.h>
#include <ints.h>

#include <system.h>

void keyboard_callback();

u8 qwerty_keymap[128] = 
"\0"
"\033" // Esc (27)
"1234567890-=\b"
"\t" // Tab
"qwertyuiop[]"
"\n" // Enter
"\0" // Ctrl
"asdfghjkl;'`"
"\0" // Left Shift
"\\zxcvbnm,./"
"\0" // Right Shift
"*"
"\0" // Alt
" " // Space
"\0" // CapsLock
"\0\0\0\0\0\0\0\0\0\0" // Function keys F1 - F10
"\0" // NumLock
"\0" // Scroll Lock
"\0" // Home
"\0" // Up
"\0" // PgUp
"\0" // Left
"\0" // Right
"\0" // End
"\0" // Down
"\0" // PgDn
"\0" // Insert
"\0" // Delete
"\0\0\0"
"\0" // F11
"\0" // F12
;

void keyboard_callback() {
	// get scancode from keyboard's data buffer
	u8 scancode = in(0x60);

	// check the top bit
	if ((scancode & 0x80) != 0x00) {
		// a key was released
	}
	else {
		// a key was pressed
		// repetition occurs if a key is held down
		fb_putchar(qwerty_keymap[scancode]);
	}
}

void keyboard_init() {
	irq_new_routine(1, keyboard_callback);
}
