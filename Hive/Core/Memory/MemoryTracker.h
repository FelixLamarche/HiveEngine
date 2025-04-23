#pragma once
#include <mutex>
namespace hive
{
    /**
     * @brief Thread safe memory tracker.
     * Notify when a memory leak is detected at the end of the program
     */
    class HIVE_API MemoryTracker
    {
        std::vector<void*> p_allocations;
        std::mutex p_allocations_mutex;
    public:
        void Init();
        void Shutdown();

        void Track(void*);
        void Untrack(void*);
    };


    /**
     * @brief Get the global memory tracker (Lazy loaded)
     * @return the global memory tracker
     */
    HIVE_API MemoryTracker &GetMemoryTracker();

    HIVE_API void SetMemoryTracker(MemoryTracker *memory_tracker);
}
