//
// Created by lapor on 4/9/2025.
//

#include "Object.h"

void * hive::Object::operator new(size_t size, const char *file, int line)
{
    return hive_allocate(size, HIVE_ALLOC_CAT_ENGINE, file, line);
}

void hive::Object::operator delete(void *ptr)
{
    hive_free(ptr);
}
