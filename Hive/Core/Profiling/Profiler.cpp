#include "Profiler.h"

namespace hive
{
    HIVE_API void (*PFN_profile_init)() = nullptr;
    HIVE_API void (*PFN_profile_shutdown)() = nullptr;
    HIVE_API ProfileCtx (*PFN_profile_zone_begin)(const ProfileLocationData* data) = nullptr;
    HIVE_API void (*PFN_profile_zone_end)(ProfileCtx ctx) = nullptr;
    HIVE_API void (*PFN_profile_memory_alloc)(const void* ptr, const char* pool_name, uint32 size) = nullptr;
    HIVE_API void (*PFN_profile_memory_free)(const void* ptr) = nullptr;

    void profile_init()
    {
        HIVE_ASSERT(PFN_profile_init != nullptr, "");
        HIVE_LOG_DEBUG("Profiler init");
        PFN_profile_init();
    }

    void profile_shutdown()
    {
        HIVE_ASSERT(PFN_profile_shutdown != nullptr, "");
        HIVE_LOG_DEBUG("Profiler shutdown");
        PFN_profile_shutdown();
    }

    ProfileCtx profile_zone_begin(const ProfileLocationData* data)
    {
        HIVE_ASSERT(PFN_profile_zone_begin != nullptr, "");
        return PFN_profile_zone_begin(data);
    }

    void profile_zone_end(const ProfileCtx &ctx)
    {
        HIVE_ASSERT(PFN_profile_zone_end != nullptr, "");
        PFN_profile_zone_end(ctx);
    }

    void profile_memory_alloc(const void* ptr, const char* pool_name, uint32 size)
    {
        HIVE_ASSERT(PFN_profile_memory_alloc != nullptr, "");
        PFN_profile_memory_alloc(ptr, pool_name, size);
    }


    void profile_memory_free(const void* ptr)
    {
        HIVE_ASSERT(PFN_profile_memory_free != nullptr, "");
        PFN_profile_memory_free(ptr);
    }
}

