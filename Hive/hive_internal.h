#pragma once
#include <Core/Libraries/LibraryManager.h>

//WARNING only the engine should include this file. The user should not
namespace hive
{

    class HiveLibrary
    {
    public:
        MemoryTracker tracker;
        LogManager logManager;
        LibraryManager lib_manager;

#ifdef HIVE_PROFILER_ENABLED
        void* profiler_lib = nullptr;
#endif
    };

    extern HiveLibrary g_hive;
}
