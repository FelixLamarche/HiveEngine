#include "MemoryAllocator.h"
#include "MemoryTracker.h"

#include <cstdlib>
#include <cstring>
#include <hive_internal.h>

#ifdef HIVE_TRACK_PROFILE_FULL

struct AllocationData
{
    size_t size;
    AllocationCategory category;
    const std::string file;
    int line;

    AllocationData(size_t _size, AllocationCategory _category, const char* _file, int _line) : size(_size), category(_category), file(_file), line(_line) {}
};

void* hive_allocate(size_t size, AllocationCategory category, const char* file, int line)
{
    const auto actual_ptr = static_cast<char*>(std::malloc(size + sizeof(AllocationData)));
    std::memset(actual_ptr, 0, size + sizeof(AllocationData));

    if (actual_ptr == nullptr)
    {
        HIVE_LOG_ERROR("Failed to allocate on the heap");
        return nullptr;
    }

    auto *data = reinterpret_cast<AllocationData*>(actual_ptr);
    new (data) AllocationData(size, category, file, line);
    const auto ptr = actual_ptr + sizeof(AllocationData);

    hive::g_hive.tracker.Track(actual_ptr);

    return ptr;
}

void hive_free(void* ptr)
{
    const auto actual_ptr = static_cast<char*>(ptr) - sizeof(AllocationData);
    AllocationData *data = reinterpret_cast<AllocationData*>(actual_ptr);
    hive::g_hive.tracker.Untrack(actual_ptr);


    std::memset(actual_ptr, 0, data->size + sizeof(AllocationData));
    std::free(actual_ptr);
}


void hive_report_allocation_data(void* ptr)
{
    //TODO use actual LogManager

    auto data = reinterpret_cast<AllocationData*>(ptr);


    std::printf("Allocated at %s, at line %d\n", data->file.c_str(), data->line);
}


#endif
