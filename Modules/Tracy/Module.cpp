#include "Core/Profiling/Profiler.h"

#include "tracy/TracyC.h"
#include <client/TracyProfiler.hpp>
void tracy_init()
{
    ___tracy_startup_profiler();
}

void tracy_shutdown()
{
    ___tracy_shutdown_profiler();
}

hive::ProfileCtx tracy_zone_begin(const hive::ProfileLocationData *data)
{
    hive::ProfileCtx ctx;
    ctx.active = true;

    const auto id = tracy::GetProfiler().GetNextZoneId();
    ctx.id = id;

     {
        TracyQueuePrepareC( tracy::QueueType::ZoneValidation );
        tracy::MemWrite( &item->zoneValidation.id, id );
        TracyQueueCommitC( zoneValidationThread );
    }
    {
        TracyQueuePrepareC( tracy::QueueType::ZoneBegin );
        tracy::MemWrite( &item->zoneBegin.time, tracy::Profiler::GetTime() );
        tracy::MemWrite( &item->zoneBegin.srcloc, (uint64_t)data ); //We can do that since ___tracy_source_location_data is the same struct as ProfileLocationData
        TracyQueueCommitC( zoneBeginThread );
    }

    return ctx;
}

void tracy_zone_end(hive::ProfileCtx ctx)
{
    if (!ctx.active) return;
    {
        TracyQueuePrepareC( tracy::QueueType::ZoneValidation );
        tracy::MemWrite( &item->zoneValidation.id, ctx.id );
        TracyQueueCommitC( zoneValidationThread );
    }
    {
        TracyQueuePrepareC( tracy::QueueType::ZoneEnd );
        tracy::MemWrite( &item->zoneEnd.time, tracy::Profiler::GetTime() );
        TracyQueueCommitC( zoneEndThread );
    }
}

void tracy_memory_alloc(const void* ptr, const char* pool_name, uint32 size)
{
    tracy::Profiler::MemAllocNamed(ptr, size, false, pool_name);
}

void tracy_memory_free(const void* ptr)
{
    tracy::Profiler::MemFree(ptr, false);
}


extern "C" void InitModule()
{
    hive::PFN_profile_init = tracy_init;
    hive::PFN_profile_shutdown = tracy_shutdown;
    hive::PFN_profile_zone_begin = tracy_zone_begin;
    hive::PFN_profile_zone_end = tracy_zone_end;
    hive::PFN_profile_memory_alloc = tracy_memory_alloc;
    hive::PFN_profile_memory_free = tracy_memory_free;
}

extern "C" void ShutdownModule()
{

    hive::PFN_profile_init = nullptr;
    hive::PFN_profile_shutdown = nullptr;
    hive::PFN_profile_zone_begin = nullptr;
    hive::PFN_profile_zone_end = nullptr;
    hive::PFN_profile_memory_alloc = nullptr;
    hive::PFN_profile_memory_free = nullptr;
}