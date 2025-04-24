#include "Core/Logging/LogManager.h"
#include "Display/DisplayType.h"
#include "DisplayGLFW.h"
// #include "NativeHelperGLFW.h"

#include <stdexcept>

#include "NativeHelperGLFW.h"

namespace hive::glfw
{
    Display *display_create(const DisplayCreateInfo &create_info)
    {
        Display *display = static_cast<Display *>(HV_NEW(sizeof(Display), HIVE_ALLOC_CAT_ENGINE));
        display->p_width = create_info.width;
        display->p_height = create_info.height;
        display->p_title = create_info.title.c_str();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        display->p_window = glfwCreateWindow(display->p_width, display->p_height, display->p_title, nullptr, nullptr);

        if (display->p_window == nullptr)
            HIVE_LOG_ERROR("Failed to create GLFW window");
        else
        {
            glfwSetWindowUserPointer(display->p_window, &create_info.event_manager);
        }

        display_setup_callback(display);

        return display;
    }

    void display_destroy(Display *display)
    {
        glfwDestroyWindow(display->p_window);

        HV_FREE(display);
    }

    void display_setup_callback(const Display *display)
    {
        glfwSetWindowCloseCallback(display->p_window, [](GLFWwindow *window)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));

            Event event;
            manager->EventPush(event, EVENT_CODE_APPLICATION_QUIT);
        });

        glfwSetFramebufferSizeCallback(display->p_window, [](GLFWwindow *window, int width, int height)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;
            event.data.u16[0] = width;
            event.data.u16[1] = height;

            manager->EventPush(event, EVENT_CODE_WINDOW_RESIZED);
        });

        glfwSetKeyCallback(display->p_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            auto *manager = static_cast<EventManager *>(glfwGetWindowUserPointer(window));
            Event event;

            event.data.u16[0] = key;
            manager->EventPush(event, action == GLFW_PRESS ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED);
        });
    }


    void *display_get_native_display(Display *display)
    {
        return GetNativeDisplay(display->p_window);
    }

    void *display_get_native_window(Display *display)
    {
        return GetNativeWindow(display->p_window);
    }

    void display_poll_event()
    {
        glfwPollEvents();
    }
}
