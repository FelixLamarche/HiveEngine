#include "Application.h"
#include <rendering/vulkan/GraphicsDeviceVulkan.h>

#include <chrono>
#include <stdexcept>

hive::Application::Application(const ApplicationConfig &config) : input_(), memory_(), window_(config.window_config), active_camera_(nullptr), device_vulkan_(nullptr)
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

	// TODO implement a better way to handle time
    const auto startTime = std::chrono::high_resolution_clock::now();
    auto prevTime = startTime;
    f32 deltaTime = 0.0f;
    while (!app_should_close_ && !window_.shouldClose())
    {
		const auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<f32, std::chrono::seconds::period>(currentTime - prevTime).count();
		prevTime = currentTime;

		input_.on_update(deltaTime);
        window_.pollEvents();
        on_update(deltaTime);
    }

    on_destroy();
}
