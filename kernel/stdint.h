#ifndef STDINT_H
#define STDINT_H

/* Signed */
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long int int64_t;

/* Unsigned */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

/* Types for pointers */
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;

/* Limits of integer types*/

/* Minimum of signed integer types */
#define INT8_MIN   (-1-0x7f)
#define INT16_MIN  (-1-0x7fff)
#define INT32_MIN  (-1-0x7fffffff)
#define INT64_MIN  (-1-0x7fffffffffffffffl)

/* Maximum of signed integer types */
#define INT8_MAX   (0x7f)
#define INT16_MAX  (0x7fff)
#define INT32_MAX  (0x7fffffff)
#define INT64_MAX  (0x7fffffffffffffffl)

/* Maximum of unsigned integer types */
#define UINT8_MAX  (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffffu)
#define UINT64_MAX (0xfffffffffffffffful)

#endif /* STDINT_H */
