#pragma once

#include <stdio.h>
#include <stdlib.h>

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define DEBUG_BREAK() __builtin_trap()
#else
    #define DEBUG_BREAK() ((void)0)
#endif

#define HIVE_ENABLE_ASSERTS
#ifdef HIVE_ENABLE_ASSERTS
    #define HIVE_ASSERT(x, ...) \
    do { \
    if (!(x)) { \
    fprintf(stderr, "Assertion Failed: %s\n", #x); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
    DEBUG_BREAK(); \
    abort(); \
    } \
    } while (0)
#else
    #define ASSERT(x, ...)
#endif