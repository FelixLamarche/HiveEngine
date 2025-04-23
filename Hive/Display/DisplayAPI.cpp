#include "DisplayAPI.h"
#include "Core/Logging/LogManager.h"

std::vector<hive::DisplayAPI*> g_display_api;
hive::DisplayAPI *g_current_display_api = nullptr;
void hive::DisplayRegisterAPI(DisplayAPI *api)
{
    //TODO: verify that all the function are setup
    g_display_api.push_back(api);
}

void hive::DisplaySetBackend(const std::string &backend)
{
    if (g_current_display_api != nullptr)
    {
        HIVE_LOG_WARN("Attempting to reset the display backend to another one. This could cause some serious problem with already allocated Display");
    }

    for(const auto api : g_display_api)
    {
        if(api->backend == backend)
        {
            g_current_display_api = api;
            break;
        }
    }

    if (g_current_display_api == nullptr)
    {
        HIVE_LOG_ERROR("Display API not found");
    }
}

hive::Display* hive::display_create(const DisplayCreateInfo &create_info)
{
    //TODO some assertion
    return g_current_display_api->CreateDisplay(create_info);
}

void hive::display_destroy(Display *display)
{
    g_current_display_api->DisplayDestroy(display);
}

void* hive::DisplayGetNativeDisplay(Display *display)
{
    return g_current_display_api->GetNativeDisplay(display);
}


void* hive::DisplayGetNativeWindow(Display *display)
{
    return g_current_display_api->GetNativeWindow(display);
}

void hive::DisplayPollEvent()
{
    g_current_display_api->PollEvent();
}
