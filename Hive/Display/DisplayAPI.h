#pragma once
#include "DisplayType.h"
namespace hive
{
    HIVE_API void DisplayRegisterAPI(DisplayAPI *api);

    HIVE_API void DisplaySetBackend(const std::string &backend);
    HIVE_API Display* display_create(const DisplayCreateInfo &create_info);
    HIVE_API void display_destroy(Display *display);

    HIVE_API void* DisplayGetNativeDisplay(Display *display);
    HIVE_API void* DisplayGetNativeWindow(Display *display);

    HIVE_API void DisplayPollEvent();
}
