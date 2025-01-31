#pragma once
#include <core/Window.h>
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
        [[nodiscard]] bool isKeyPressed(InputKey input) const override;
        [[nodiscard]] glm::vec2 getMousePosition() const override;
        [[nodiscard]] glm::vec2 getMouseOffset() const override;
		void unlockCursor() override;

#ifdef HIVE_BACKEND_VULKAN_SUPPORTED
        void appendRequiredVulkanExtension(std::vector<const char*> &vector) const override;
        void createVulkanSurface(void *instance, void *surface_khr) const override;
#endif
    private:
        GLFWwindow* window_;
        std::array<int, static_cast<u64>(InputKey::NB_KEYS)> keys_pressed_{};
		glm::vec2 prev_mouse_pos_{}; // 0,0 is top-left corner, position in pixels
		glm::vec2 mouse_pos_{};

        void process_inputs_keyboard();
        void process_inputs_mouse();
    };
}
