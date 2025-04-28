#include "Core/Logging/ConsoleLogger.h"
#include "Core/Logging/LogManager.h"
#include "Display/DisplayAPI.h"
#include "Graphic/GFX.h"
#include "hive.h"

#include <Core/Profiling/Profiler.h>


#include "hive_internal.h"


hive::HiveLibrary hive::g_hive;

bool hive::Init()
{
    g_hive.logManager.Init();
    g_hive.tracker.Init();
    g_hive.lib_manager.Init();


    //TODO read config file


    g_hive.logManager.SetLogLevel(LOG_LEVEL_DEBUG);
    const ConsoleLogger tmp_logger;
    auto tmp_logger_id = g_hive.logManager.AddLogger(&tmp_logger);

#ifdef HIVE_PROFILER_ENABLED
    //The profiler module is a special case of module and need to be managed manually
    g_hive.profiler_lib = hive::LibraryManager::LoadLibraryUnmanaged("Tracy");
    if (g_hive.profiler_lib == nullptr)
        return false;

    hive::profile_init();
#endif


    auto glfw_lib = g_hive.lib_manager.LoadLibrary("Glfw");
    if(glfw_lib == -1)
    {
        return false;
    }
    hive::DisplaySetBackend("glfw");

    auto vulkan_lib = g_hive.lib_manager.LoadLibrary("Vulkan");
    if(vulkan_lib == -1)
    {
        return false;
    }

    hive::gfx::set_backend_api("vulkan");


    g_hive.logManager.RemoveCallback(tmp_logger_id);

    hive::ConsoleLogger *console_logger = HV_NEW_OOP ConsoleLogger();
    g_hive.logManager.AddLogger(console_logger);

    return true;
}

void hive::Shutdown()
{
    g_hive.lib_manager.Shutdown();
    g_hive.logManager.Shutdown();
    g_hive.tracker.Shutdown();

#ifdef HIVE_PROFILER_ENABLED
    hive::LibraryManager::UnloadLibraryUnmanaged(g_hive.profiler_lib);
#endif
}
