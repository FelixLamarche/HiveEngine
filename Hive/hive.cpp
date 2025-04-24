#include "Core/Logging/ConsoleLogger.h"
#include "Core/Logging/LogManager.h"
#include "Display/DisplayAPI.h"
#include "Graphic/GFX.h"
#include "hive.h"


#include "hive_internal.h"


hive::HiveLibrary hive::g_hive;

bool hive::Init()
{
    g_hive.tracker.Init();
    g_hive.logManager.Init();
    g_hive.lib_manager.Init();

    //TODO read config file


    g_hive.logManager.SetLogLevel(LOG_LEVEL_DEBUG);
    const ConsoleLogger *logger = HV_NEW_OOP ConsoleLogger();
    g_hive.logManager.AddLogger(logger);

    //TODO Load library from config file

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

    return true;
}

void hive::Shutdown()
{
    g_hive.lib_manager.Shutdown();
    g_hive.logManager.Shutdown();
    g_hive.tracker.Shutdown();
}
