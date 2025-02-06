#pragma once
#include <core/Window.h>
#include <core/Input.h>
#include <GLFW/glfw3.h>

#include <array>

namespace hive
{
    class WindowGLFW final : public IWindow
    {
    public:
        ~WindowGLFW() override;
        WindowGLFW(const WindowConfig &config);


        [[nodiscard]] u64 getSizeof() const override;

        [[nodiscard]] bool shouldClose() const override;

        void pollEvents() override;
        void waitEvents() const  override;
        void getFramebufferSize(i32 &width, i32 &height) const override;
		void unlockCursor() override;
        void subscribeToKeyInputs(std::function<void(InputKey, InputState)> callback) override;
		void subscribeToMouseInputs(std::function<void(f32, f32)> callback) override;
        void subscribeToMouseKeyInputs(std::function<void(MouseButton, InputState)> callback) override;
        void subscribeToMouseScrollInputs(std::function<void(f32, f32)> callback) override;


#ifdef HIVE_BACKEND_VULKAN_SUPPORTED
        void appendRequiredVulkanExtension(std::vector<const char*> &vector) const override;
        void createVulkanSurface(void *instance, void *surface_khr) const override;
#endif
    private:
        GLFWwindow* window_;
        std::function<void(InputKey, InputState)> input_key_callback_;
		std::function<void(f32, f32)> mouse_callback_;
		std::function<void(MouseButton, InputState)> mouse_key_callback_;
		std::function<void(f32, f32)> mouse_scroll_callback_;

        [[nodiscard]] static InputKey glfwKeyToInputKey(i32 key);
        void handleKeyEvent(i32 key, i32 scancode, i32 action, i32 mods) const;
		void handleMouseEvent(f64 xpos, f64 ypos) const;
		void handleMouseKeyEvent(i32 button, i32 action, i32 mods) const;
        void handleMouseScrollEvent(f64 xoffset, f64 yoffset) const;
    };
}
