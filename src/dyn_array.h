/**
 * If you're unaware, a dynamic array is basically the same thing as std::vector in C++.
 * Basically this is an array that resizes itself when you want to add to it.
 *
 * I'm so sorry for these macros.
 */

#if 0
Example {
    DYN_ARRAY(double) test;
    DYN_ARRAY_INIT(&test);

    DYN_ARRAY_PUSH(&test, 69.0);
    DYN_ARRAY_PUSH(&test, 255.0);
    DYN_ARRAY_PUSH(&test, 300.0);
    DYN_ARRAY_PUSH(&test, 400.0);

    DYN_ARRAY_REMOVE(&test, 2.0);

    DYN_ARRAY_PUSH(&test, 27.0);

    DYN_ARRAY_FOR_EACH(&test, i, { // Will output:
        printf("%f\n", i);         // 69.000000, 255.000000, 400.000000, 27.000000
    })

    DYN_ARRAY_DEINIT(&test);
}
#endif // 0

#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include "raylib.h"
#include "stdlib.h" // for size_t, NULL

// Macros galore. Beware.

// Dynamic array "type"
#define DYN_ARRAY(Type) \
    struct { \
        Type * data; \
        size_t length, cap; \
    }

// Initialise the array (not strictly needed if you initialise the array as zeros e.g.: `DYN_ARRAY(int) foo = {0}`)
#define DYN_ARRAY_INIT(array) \
    (array)->data = MemAlloc(sizeof(*((array)->data))); \
    (array)->cap = 1; \
    (array)->length = 0

// Push to the top of the array. Grows the array if needed.
#define DYN_ARRAY_PUSH(array, value) \
    if ((array)->length == (array)->cap) { \
        (array)->data = MemRealloc((array)->data, sizeof(*((array)->data)) * (array)->cap + sizeof(*((array)->data))); \
        (array)->cap += 1; \
    } \
    (array)->data[(array)->length] = value; \
    (array)->length += 1

// Free the data in the array and zero it out. Could be replaced with a free and a memset.
#define DYN_ARRAY_DEINIT(array) \
    MemFree((array)->data); \
    (array)->data = NULL; \
    (array)->length = 0; \
    (array)->cap = 0

// Remove the nth element from an array
#define DYN_ARRAY_REMOVE(array, n) \
    for (size_t _da_i = (n); _da_i < (array)->length - 1; ++_da_i) \
        (array)->data[_da_i] = (array)->data[_da_i + 1]; \
    (array)->length -= 1

/*
 * WARN(Tuhkis): You cant redefine `iterator + _num` inside your expression, but you can and read it
 * NOTE(Tuhkis): the expression is an argument to the macro.
 */
#define DYN_ARRAY_FOR_EACH(array, iterator, expression) \
    {typeof(*((array)->data)) (iterator) = (array)->data[0]; \
    for ( \
        size_t (iterator##_num) = 0; \
        (iterator##_num) < (array)->length; \
        (iterator) = (array)->data[++(iterator##_num)] \
    ) expression}

#endif /* DYN_ARRAY_H */

