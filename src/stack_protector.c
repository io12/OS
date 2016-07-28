#include <ints.h>
#include <kprintf.h>
#include <system.h>

u32 __stack_chk_guard = 0xe2dee396;

void __stack_chk_fail() {
	kprintf(PL_ALL, "STACK SMASHING DETECTED\n");

	permahalt();
}
