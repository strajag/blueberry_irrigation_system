#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include "Arduino.h"
#include "stddef.h"

/* TYPE */
#define FALSE (0)
#define TRUE !FALSE
#undef NULL
/*#define NULL ((void *)0)*/
#define NULL (0)

typedef uint8_t bool_t;
typedef float f32_t;
/* TYPE */

/* DEBUGGING */
#define DEBUGGING 0
#define DEBUG_PRINTF_MAX_SIZE 64

#if DEBUGGING
#define malloc(x) debug_malloc(x)
#define free(x) debug_free(x)

#if(DEBUGGING == 1) /* DEBUGGING == 1 */
#define debug_log(args) (debug_printf args)
#else
#define debug_log(args) (debug_printf("LOG: %s:%d ", __FILE__, __LINE__), debug_printf args)
#endif /* DEBUGGING == 1 */

#else
#define debug_log(args) ((void)0)
#endif /* DEBUGGING */

extern void *debug_malloc(size_t size);
extern void debug_free(void *memory);
extern void debug_printf(const char *format, ...);
extern void debug_printf_definition(char *string);
/* DEBUGGING */

#endif /* MY_LIBRARY_H */
