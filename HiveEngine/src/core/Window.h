#pragma once
#include <hvpch.h>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "core/Input.h"
#include <rendering/RendererPlatform.h>

namespace hive
{
    struct WindowConfig
    {
        enum class WindowType
        {
            Native, GLFW, Raylib, NONE
        };

        WindowType type;
        u16 width, height;
        const char* title;

    };

    class IWindow
    {
    public:
        virtual ~IWindow()= default;
        [[nodiscard]] virtual u64 getSizeof() const = 0;
        [[nodiscard]] virtual bool shouldClose() const = 0;
        virtual void pollEvents() = 0;
        virtual void waitEvents() const = 0;
        virtual void getFramebufferSize(i32& width, i32 &height) const = 0;
		virtual void unlockCursor() = 0;
        virtual void subscribeToKeyInputs(std::function<void(InputKey, InputState)> callback) = 0;
        virtual void subscribeToMouseInputs(std::function<void(f32, f32)> callback) = 0;
        virtual void subscribeToMouseKeyInputs(std::function<void(MouseButton, InputState)> callback) = 0;
		virtual void subscribeToMouseScrollInputs(std::function<void(f32, f32)> callback) = 0;


#ifdef HIVE_BACKEND_VULKAN_SUPPORTED
        virtual void appendRequiredVulkanExtension(std::vector<const char*> &vector) const = 0;
        virtual void createVulkanSurface(void* instance, void *surface_khr) const = 0;
#endif
    };


    class Window
    {
    public:
        explicit Window(const WindowConfig &config);
        ~Window();

        [[nodiscard]] bool shouldClose() const;
        void pollEvents();
        void waitEvents() const ;

        void getFramebufferSize(i32& width, i32 &height) const;
        void unlockCursor();
        void subscribeToKeyInputs(std::function<void(InputKey, InputState)> callback);
		void subscribeToMouseInputs(std::function<void(f32, f32)> callback);
		void subscribeToMouseKeyInputs(std::function<void(MouseButton, InputState)> callback);
        void subscribeToMouseScrollInputs(std::function<void(f32, f32)> callback);

#ifdef HIVE_BACKEND_VULKAN_SUPPORTED
        void appendRequiredVulkanExtension(std::vector<const char*> &vector) const;
        void createVulkanSurface(void* instance, void *surface_khr) const;
#endif
    private:
        IWindow* window_handle{};
    };
}
