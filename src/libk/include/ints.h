/* ints.h */

// Similar to stdint.h

#ifndef _INTS_H_
#define _INTS_H_

// signed ints
typedef signed char		int8_t;
typedef signed short int	int16_t;
typedef signed int		int32_t;
#if __WORDSIZE == 64
typedef long int		int64_t;
#else
__extension__
typedef long long int		int64_t;
#endif

// unsigned ints
typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
#if __WORDSIZE == 64
typedef unsigned long int	uint64_t;
#else
__extension__
typedef unsigned long long int	uint64_t;
#endif

// shorthand definitions
typedef int8_t			i8;
typedef int16_t			i16;
typedef int32_t			i32;
typedef int64_t			i64;
typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;
typedef uint64_t		u64;

#endif
