#pragma once
#include "Display/DisplayType.h"
#include "GLFW/glfw3.h"

namespace hive
{
    struct Display
    {
        GLFWwindow *p_window;

        uint16 p_width, p_height;
        const char* p_title;
   };
}

namespace hive::glfw
{
    Display *display_create(const DisplayCreateInfo &create_info);
    void display_setup_callback(const Display *display);

    void display_destroy(Display *display);

    void *display_get_native_display(Display *display);

    void *display_get_native_window(Display *display);

    void display_poll_event();


}
