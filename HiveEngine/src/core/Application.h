#pragma once
#include <optional>

#include "core/Input.h"
#include "core/Memory.h"
#include "core/Logger.h"
#include "core/Window.h"
#include "rendering/Camera.h"

namespace hive::vk
{
    class GraphicsDevice_Vulkan;
}

namespace hive
{

    struct ApplicationConfig
    {
        Logger::LogLevel log_level;
        WindowConfig window_config;
    };


    class Application
    {
    public:
        explicit Application(const ApplicationConfig &config);

        virtual ~Application();
        void run();
		void setActiveCamera(Camera* camera) { active_camera_ = camera; }

    protected:
        virtual bool on_init() = 0;
        virtual bool on_update(float delta_time) = 0;
        virtual bool on_destroy() = 0;

		Input input_;
        Memory memory_;
        Window window_;
        Camera* active_camera_;
        // GraphicsDevice *device_ = nullptr;
        vk::GraphicsDevice_Vulkan *device_vulkan_;

        bool app_should_close_ = false;
    };
}
