#define GLFW_INCLUDE_VULKAN
#include <core/Window.h>
#include <core/Logger.h>
#include <GLFW/glfw3.h>
#include "window_glfw.h"

#include <vector>

hive::WindowGLFW::~WindowGLFW()
{
    LOG_INFO("Destroying GLFW Window");
    glfwDestroyWindow(window_);
}

hive::WindowGLFW::WindowGLFW(const WindowConfig &config)
{
    if (!glfwInit())
    {
        //TODO: error handling
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window_ = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);

    // Lock cursor to window
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!window_)
    {
        //TODO: error handling
    }
}

u64 hive::WindowGLFW::getSizeof() const
{
    return sizeof(WindowGLFW);
}

bool hive::WindowGLFW::shouldClose() const
{
    return glfwWindowShouldClose(window_);
}

void hive::WindowGLFW::pollEvents()
{
    glfwPollEvents();
    process_inputs_keyboard();
    process_inputs_mouse();
}

void hive::WindowGLFW::waitEvents() const
{
    glfwWaitEvents();
}

void hive::WindowGLFW::getFramebufferSize(i32 &width, i32 &height) const
{
    glfwGetFramebufferSize(window_, &width, &height);
}

// TODO: Implement Inputs correctly later
bool hive::WindowGLFW::isKeyPressed(InputKey input) const
{
	if (keys_pressed_[static_cast<u64>(input)])
	{
		return true;
	}
    return false;
}

glm::vec2 hive::WindowGLFW::getMousePosition() const
{
    return mouse_pos_;
}

glm::vec2 hive::WindowGLFW::getMouseOffset() const
{
	return mouse_pos_ - prev_mouse_pos_;
}

void hive::WindowGLFW::unlockCursor()
{
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

#ifdef HIVE_BACKEND_VULKAN_SUPPORTED
void hive::WindowGLFW::appendRequiredVulkanExtension(std::vector<const char *> &vector) const
{
    u32 count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    for (int i = 0; i < count; i++)
    {
        vector.emplace_back(extensions[i]);
    }
}

void hive::WindowGLFW::createVulkanSurface(void *instance, void *surface_khr) const
{
    auto vk_instance = static_cast<VkInstance>(instance);
    auto vk_surface = static_cast<VkSurfaceKHR*>(surface_khr);
    glfwCreateWindowSurface(vk_instance, window_, nullptr, vk_surface);
}
#endif

void hive::WindowGLFW::process_inputs_keyboard()
{
	const static std::vector<std::pair<i32, InputKey>> key_map = {
		{GLFW_KEY_W, InputKey::W},
		{GLFW_KEY_S, InputKey::S},
		{GLFW_KEY_A, InputKey::A},
		{GLFW_KEY_D, InputKey::D},
		{GLFW_KEY_E, InputKey::E},
		{GLFW_KEY_Q, InputKey::Q},
		{GLFW_KEY_ESCAPE, InputKey::ESC}
	};

	for (const auto& kv: key_map)
	{
		auto action = glfwGetKey(window_, kv.first);
		if (action == GLFW_PRESS)
		{
			keys_pressed_[static_cast<u32>(kv.second)] = 1;
		}
		else if (action == GLFW_RELEASE)
		{
			keys_pressed_[static_cast<u32>(kv.second)] = 0;
		}
	}
}

void hive::WindowGLFW::process_inputs_mouse()
{
    static bool initialized_mouse = false;
	double x, y;
	glfwGetCursorPos(window_, &x, &y);
	prev_mouse_pos_ = mouse_pos_;
	mouse_pos_ = { static_cast<f32>(x), static_cast<f32>(y) };

	// Prevents a huge offset when the mouse is first initialized
	if (!initialized_mouse)
	{
		prev_mouse_pos_ = mouse_pos_;
		initialized_mouse = true;
	}
}
