#include "Application.h"
#include <rendering/vulkan/GraphicsDeviceVulkan.h>

#include <chrono>
#include <stdexcept>

hive::Application::Application(const ApplicationConfig &config) : input_(), memory_(), window_(config.window_config), device_vulkan_(nullptr)
{
    device_vulkan_ = Memory::createObject<vk::GraphicsDevice_Vulkan, Memory::RENDERER>(window_);

	input_.subscribeToWindowEvents(window_);
}

hive::Application::~Application()
{
    Memory::destroyObject<vk::GraphicsDevice_Vulkan, Memory::RENDERER>(device_vulkan_);
}



void hive::Application::run()
{
    on_init();

    while (!app_should_close_ && !window_.shouldClose())
    {
		input_.on_update(0.0f);
        window_.pollEvents();
        on_update(0);
    }

    on_destroy();
}
