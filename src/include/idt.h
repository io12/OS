/* idt.h */

#ifndef _IDT_H_
#define _IDT_H_

void idt_init();
void lidt(void* idt);

#endif
