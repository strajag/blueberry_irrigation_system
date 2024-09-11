#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"

/* SSL_TYPE */
#define FALSE (0)
#define TRUE !FALSE

typedef uint8_t bool_t;
typedef float f32_t;
typedef double f64_t;
/* SSL_TYPE */

/* SSL_DEBUG */
#ifdef SSL_DEBUG

#define malloc(x) ssl_debug_malloc(size)
#define free(x) ssl_debug_free(pointer)

#if(SSL_DEBUG == 0) /* SSL_DEBUG == 0 */

#define ssl_debug_log(args) (printf args)

#else

#define ssl_debug_log(args) (printf("LOG: %s:%d ", __FILE__, __LINE__), printf args)

#endif /* SSL_DEBUG == 0 */

#else

#define ssl_debug_log(args) ((void)0)

#endif /* SSL_DEBUG */

extern void *ssl_debug_malloc(size_t size);
extern void ssl_debug_free(void *pointer);
/* SSL_DEBUG */

#endif /* MY_LIBRARY_H */
