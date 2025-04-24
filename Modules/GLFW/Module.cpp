#include "Display/DisplayAPI.h"
#include "DisplayGLFW.h"
#include "GLFW/glfw3.h"

hive::DisplayAPI glfw_api
{
    "glfw",
    hive::glfw::display_create,
    hive::glfw::display_destroy,
    hive::glfw::display_get_native_display,
    hive::glfw::display_get_native_window,
    hive::glfw::display_poll_event,
};

extern "C" HIVE_API void InitModule()
{
    HIVE_LOG_DEBUG("Init GLFW module");
    glfwInit();

    hive::DisplayRegisterAPI(&glfw_api);
}

extern "C" HIVE_API void ShutdownModule()
{
    HIVE_LOG_DEBUG("Shutdown GLFW module");
    glfwTerminate();
}
