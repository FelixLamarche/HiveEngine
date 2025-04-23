#pragma once
#include "GraphicTypes.h"

namespace hive::gfx
{
    HIVE_API void RegisterAPI(GraphicAPI *api);
    HIVE_API void SetBackend(const std::string &backend);

    HIVE_API GraphicDevice* device_create(const DeviceCreateInfo &create_info);
    HIVE_API void device_destroy(GraphicDevice *device);
}
