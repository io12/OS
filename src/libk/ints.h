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

#endif
