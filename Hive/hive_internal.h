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
    };

    extern HiveLibrary g_hive;
}
