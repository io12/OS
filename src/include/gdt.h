/* gdt.h */

#ifndef _GDT_H_
#define _GDT_H_

void gdt_init();
void lgdt(void* gdt);

#endif
