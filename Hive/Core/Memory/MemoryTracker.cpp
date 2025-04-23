#include "MemoryTracker.h"

#include <algorithm>
#include <hive_internal.h>
#include <iostream>


void hive::MemoryTracker::Init()
{
}

void hive::MemoryTracker::Shutdown()
{
    if (!p_allocations.empty())
    {
        std::cout << "Memory leaks detected" << std::endl;
        for (const auto ptr : p_allocations)
        {
            hive_report_allocation_data(ptr);
        }
    }
}

void hive::MemoryTracker::Track(void* ptr)
{
    std::lock_guard guard(p_allocations_mutex);
    p_allocations.push_back(ptr);
}

void hive::MemoryTracker::Untrack(void* ptr)
{
    std::lock_guard guard(p_allocations_mutex);
    if (const auto it = std::ranges::find(p_allocations, ptr); it != p_allocations.end())
    {
        p_allocations.erase(it);
    }
}

hive::MemoryTracker& hive::GetMemoryTracker()
{
    return g_hive.tracker;
}





