#pragma once

//The idea here is that you can create different data structure that will hold the data that you want to keep track.
//Adding new profile should not interfere with other profile.
//The profile is determined at compile time by defining the appropriate option

#include "MemoryTracker.h"
enum AllocationCategory
{
    HIVE_ALLOC_CAT_ENGINE,
    HIVE_ALLOC_CAT_GRAPHIC,
    HIVE_ALLOC_CAT_OTHER

};

#ifdef HIVE_TRACK_PROFILE_FULL

HIVE_API void* hive_allocate(size_t size, AllocationCategory category, const char* file, int line);
HIVE_API void hive_free(void* ptr);
HIVE_API void hive_report_allocation_data(void* ptr);

// #define HV_NEW(type, count, cat) (type*)hive_allocate(hive::GetMemoryTracker(), sizeof(type) * count, cat, __FILE__, __LINE__)
// #define HV_FREE(ptr) hive_free(hive::GetMemoryTracker(), ptr)
//
#define HV_NEW(size, cat) hive_allocate(size, cat, __FILE__, __LINE__)
#define HV_FREE(ptr) hive_free(ptr)
#define HV_NEW_OOP new (__FILE__, __LINE__)
#define HV_FREE_OOP delete
#endif

#ifdef HIVE_TRACK_PROFILE_DEBUG

#endif

#ifdef HIVE_TRACK_PROFILE_RELEASE

#endif
