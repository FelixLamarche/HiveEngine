#pragma once

namespace hive
{
    class HIVE_API Object
    {
    public:
        void* operator new(size_t size, const char* file, int line);
        void operator delete(void* ptr);
    };
}