#define GLFW_INCLUDE_VULKAN
#include <core/Window.h>
#include <core/Logger.h>
#include <GLFW/glfw3.h>
#include "window_glfw.h"



hive::WindowGLFW::~WindowGLFW()
{
    LOG_INFO("Destroying GLFW Window");
    glfwDestroyWindow(window_);
}

hive::WindowGLFW::WindowGLFW(const WindowConfig& config) 
	: window_(nullptr), input_key_callback_(nullptr), mouse_callback_(nullptr), mouse_key_callback_(nullptr), mouse_scroll_callback_(nullptr)
{
    if (!glfwInit())
    {
        //TODO: error handling
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window_ = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);

    if (!window_)
    {
        //TODO: error handling
    }

    // Lock cursor to window
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window_, this);

    // Input Callbacks
    glfwSetKeyCallback(window_, [](GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
    {
        auto window_glfw = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
        window_glfw->handleKeyEvent(key, scancode, action, mods);
    });
    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, f64 xpos, f64 ypos)
    {
		auto window_glfw = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
		window_glfw->handleMouseEvent(xpos, ypos);
    });
	glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto window_glfw = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
		window_glfw->handleMouseKeyEvent(button, action, mods);
	});
    glfwSetScrollCallback(window_, [](GLFWwindow* window, f64 xoffset, f64 yoffset)
    {
        auto window_glfw = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
        window_glfw->handleMouseScrollEvent(xoffset, yoffset);
    });
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
}

void hive::WindowGLFW::waitEvents() const
{
    glfwWaitEvents();
}

void hive::WindowGLFW::getFramebufferSize(i32 &width, i32 &height) const
{
    glfwGetFramebufferSize(window_, &width, &height);
}

void hive::WindowGLFW::unlockCursor()
{
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void hive::WindowGLFW::subscribeToKeyInputs(std::function<void(InputKey, InputState)> callback)
{
	if (input_key_callback_)
	{
		LOG_WARN("Overwriting existing input key callback");
	}
	input_key_callback_ = callback;
}

void hive::WindowGLFW::subscribeToMouseInputs(std::function<void(f32, f32)> callback)
{
	if (mouse_callback_)
	{
		LOG_WARN("Overwriting existing mouse callback");
	}
	mouse_callback_ = callback;
}

void hive::WindowGLFW::subscribeToMouseKeyInputs(std::function<void(MouseButton, InputState)> callback)
{
    if (mouse_key_callback_)
    {
		LOG_WARN("Overwriting existing mouse key callback");
    }
	mouse_key_callback_ = callback;
}

void hive::WindowGLFW::subscribeToMouseScrollInputs(std::function<void(f32, f32)> callback)
{
	if (mouse_scroll_callback_)
	{
		LOG_WARN("Overwriting existing mouse scroll callback");
	}
	mouse_scroll_callback_ = callback;
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

hive::InputKey hive::WindowGLFW::glfwKeyToInputKey(i32 key)
{
    static const std::unordered_map<i32, InputKey> keyMap = {
        {GLFW_KEY_A, InputKey::A},
        {GLFW_KEY_B, InputKey::B},
        {GLFW_KEY_C, InputKey::C},
        {GLFW_KEY_D, InputKey::D},
        {GLFW_KEY_E, InputKey::E},
        {GLFW_KEY_F, InputKey::F},
        {GLFW_KEY_G, InputKey::G},
        {GLFW_KEY_H, InputKey::H},
        {GLFW_KEY_I, InputKey::I},
        {GLFW_KEY_J, InputKey::J},
        {GLFW_KEY_K, InputKey::K},
        {GLFW_KEY_L, InputKey::L},
        {GLFW_KEY_M, InputKey::M},
        {GLFW_KEY_N, InputKey::N},
        {GLFW_KEY_O, InputKey::O},
        {GLFW_KEY_P, InputKey::P},
        {GLFW_KEY_Q, InputKey::Q},
        {GLFW_KEY_R, InputKey::R},
        {GLFW_KEY_S, InputKey::S},
        {GLFW_KEY_T, InputKey::T},
        {GLFW_KEY_U, InputKey::U},
        {GLFW_KEY_V, InputKey::V},
        {GLFW_KEY_W, InputKey::W},
        {GLFW_KEY_X, InputKey::X},
        {GLFW_KEY_Y, InputKey::Y},
        {GLFW_KEY_Z, InputKey::Z},

        {GLFW_KEY_0, InputKey::Num0},
        {GLFW_KEY_1, InputKey::Num1},
        {GLFW_KEY_2, InputKey::Num2},
        {GLFW_KEY_3, InputKey::Num3},
        {GLFW_KEY_4, InputKey::Num4},
        {GLFW_KEY_5, InputKey::Num5},
        {GLFW_KEY_6, InputKey::Num6},
        {GLFW_KEY_7, InputKey::Num7},
        {GLFW_KEY_8, InputKey::Num8},
        {GLFW_KEY_9, InputKey::Num9},

        {GLFW_KEY_F1, InputKey::F1},
        {GLFW_KEY_F2, InputKey::F2},
        {GLFW_KEY_F3, InputKey::F3},
        {GLFW_KEY_F4, InputKey::F4},
        {GLFW_KEY_F5, InputKey::F5},
        {GLFW_KEY_F6, InputKey::F6},
        {GLFW_KEY_F7, InputKey::F7},
        {GLFW_KEY_F8, InputKey::F8},
        {GLFW_KEY_F9, InputKey::F9},
        {GLFW_KEY_F10, InputKey::F10},
        {GLFW_KEY_F11, InputKey::F11},
        {GLFW_KEY_F12, InputKey::F12},

        {GLFW_KEY_ESCAPE, InputKey::Escape},
        {GLFW_KEY_SPACE, InputKey::Space},
        {GLFW_KEY_ENTER, InputKey::Enter},
        {GLFW_KEY_BACKSPACE, InputKey::Backspace},
        {GLFW_KEY_TAB, InputKey::Tab},
        {GLFW_KEY_CAPS_LOCK, InputKey::CapsLock},
        {GLFW_KEY_LEFT_SHIFT, InputKey::LeftShift},
        {GLFW_KEY_RIGHT_SHIFT, InputKey::RightShift},
        {GLFW_KEY_LEFT_CONTROL, InputKey::LeftCtrl},
        {GLFW_KEY_RIGHT_CONTROL, InputKey::RightCtrl},
        {GLFW_KEY_LEFT_ALT, InputKey::LeftAlt},
        {GLFW_KEY_RIGHT_ALT, InputKey::RightAlt},
        {GLFW_KEY_LEFT_SUPER, InputKey::LeftSuper},
        {GLFW_KEY_RIGHT_SUPER, InputKey::RightSuper},
        {GLFW_KEY_MENU, InputKey::Menu},
        {GLFW_KEY_PRINT_SCREEN, InputKey::PrintScreen},
        {GLFW_KEY_SCROLL_LOCK, InputKey::ScrollLock},
        {GLFW_KEY_PAUSE, InputKey::Pause},

        {GLFW_KEY_UP, InputKey::Up},
        {GLFW_KEY_DOWN, InputKey::Down},
        {GLFW_KEY_LEFT, InputKey::Left},
        {GLFW_KEY_RIGHT, InputKey::Right},

        {GLFW_KEY_KP_0, InputKey::Numpad0},
        {GLFW_KEY_KP_1, InputKey::Numpad1},
        {GLFW_KEY_KP_2, InputKey::Numpad2},
        {GLFW_KEY_KP_3, InputKey::Numpad3},
        {GLFW_KEY_KP_4, InputKey::Numpad4},
        {GLFW_KEY_KP_5, InputKey::Numpad5},
        {GLFW_KEY_KP_6, InputKey::Numpad6},
        {GLFW_KEY_KP_7, InputKey::Numpad7},
        {GLFW_KEY_KP_8, InputKey::Numpad8},
        {GLFW_KEY_KP_9, InputKey::Numpad9},
        {GLFW_KEY_KP_ADD, InputKey::NumpadAdd},
        {GLFW_KEY_KP_SUBTRACT, InputKey::NumpadSubtract},
        {GLFW_KEY_KP_MULTIPLY, InputKey::NumpadMultiply},
        {GLFW_KEY_KP_DIVIDE, InputKey::NumpadDivide},
        {GLFW_KEY_KP_DECIMAL, InputKey::NumpadDecimal},
        {GLFW_KEY_KP_ENTER, InputKey::NumpadEnter},

        {GLFW_KEY_INSERT, InputKey::Insert},
        {GLFW_KEY_DELETE, InputKey::Delete},
        {GLFW_KEY_HOME, InputKey::Home},
        {GLFW_KEY_END, InputKey::End},
        {GLFW_KEY_PAGE_UP, InputKey::PageUp},
        {GLFW_KEY_PAGE_DOWN, InputKey::PageDown},

        {GLFW_KEY_GRAVE_ACCENT, InputKey::Grave},
        {GLFW_KEY_MINUS, InputKey::Minus},
        {GLFW_KEY_EQUAL, InputKey::Equal},
        {GLFW_KEY_LEFT_BRACKET, InputKey::LeftBracket},
        {GLFW_KEY_RIGHT_BRACKET, InputKey::RightBracket},
        {GLFW_KEY_BACKSLASH, InputKey::Backslash},
        {GLFW_KEY_SEMICOLON, InputKey::Semicolon},
        {GLFW_KEY_APOSTROPHE, InputKey::Apostrophe},
        {GLFW_KEY_COMMA, InputKey::Comma},
        {GLFW_KEY_PERIOD, InputKey::Period},
        {GLFW_KEY_SLASH, InputKey::Slash}
    };

    auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        return it->second;
    }

	LOG_WARN("Unknown key pressed: %i", key);
    return InputKey::Unknown;
}

void hive::WindowGLFW::handleKeyEvent(i32 key, i32 scancode, i32 action, i32 mods) const
{
    InputState inputState = InputState::None;
	if (action == GLFW_PRESS)
	{
		inputState = InputState::Pressed;
	}
	else if (action == GLFW_RELEASE) 
	{
		inputState = InputState::Released;
	}
	else //(action == GLFW_REPEAT)
	{
		// Ignore repeat events, as they are not useful for our purposes
		// They repeat on the OS level, and not per frame
		// We only care about the initial press and release. Repeat events are to be handled by the Input class
        return;
	}

	if (input_key_callback_)
	{
	    const auto inputKey = glfwKeyToInputKey(key);
		input_key_callback_(inputKey, inputState);
	}
}

void hive::WindowGLFW::handleMouseEvent(f64 xpos, f64 ypos) const
{
	if (mouse_callback_)
	{
		mouse_callback_(static_cast<f32>(xpos), static_cast<f32>(ypos));
	}
}

void hive::WindowGLFW::handleMouseKeyEvent(i32 button, i32 action, i32 mods) const
{
    if (mouse_key_callback_)
    {
		mouse_key_callback_(static_cast<MouseButton>(button), static_cast<InputState>(action));
    }
}

void hive::WindowGLFW::handleMouseScrollEvent(f64 xoffset, f64 yoffset) const
{
	if (mouse_scroll_callback_)
	{
		mouse_scroll_callback_(static_cast<f32>(xoffset), static_cast<f32>(yoffset));
	}
}

