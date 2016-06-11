/* sanitycheck.h */

// A few checks to make sure the Makefile wasn't tampered with

#ifndef _SANITYCHECK_H_
#define _SANITYCHECK_H_

#ifndef __GNUC__
#error "Must use a compiler supporting GNU extensions"
#endif

#ifndef __ELF__
#error "Kernel must be compiled as an ELF executable"
#endif

#ifdef __cplusplus
#error "Must be compiled with a C compiler"
#endif

#ifndef __i386__
#error "Must be compiled for the i386 processor architecture"
#endif

#if __WORDSIZE == 64
#error "Must be compiled as 32 bit"
#endif

#ifdef __linux__
#warning "You are not using a cross compiler"
#endif

#endif
