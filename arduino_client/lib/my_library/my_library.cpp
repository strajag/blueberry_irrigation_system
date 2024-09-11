#include "my_library.h"
#include "stdio.h"
#include "stdarg.h"

#undef malloc
#undef free

#include "stdlib.h"

void *debug_malloc(size_t size)
{
    void *memory = malloc(size);

    debug_printf("malloc: ");
    if(memory)
        debug_printf("%p", memory);
    else
        debug_printf("null");
    debug_printf(" | %u\n", size);

    return memory;
}

void debug_free(void *memory)
{
    debug_printf("free:   ");
    if(memory)
        debug_printf("%p\n", memory);
    else
        debug_printf("null\n");

    free(memory);
}

void debug_printf(const char *format, ...)
{
    char buffer[DEBUG_PRINTF_MAX_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, DEBUG_PRINTF_MAX_SIZE, format, args);
    va_end(args);
    debug_printf_definition(buffer);
}
