#pragma once
#include "Core/Event/EventManager.h"

namespace hive
{
    class Display;
}

namespace hive
{

    /**
    * @brief Opaque handle that represent the actual window where all the magic happen.
    *
    * Abstract the interaction to the window API of the OS
    */
    struct Display;


    struct DisplayCreateInfo
    {
        // const std::string desired_backend; //all in lower case
        EventManager &event_manager;
        uint16 width, height;
        std::string title;

        DisplayCreateInfo(EventManager &_event_manager, const uint16 _width, const uint16 _height, const char* _title) : event_manager(_event_manager), width(_width), height(_height), title(_title) {}
    };

    struct DisplayAPI
    {
        const std::string backend;
        Display* (*CreateDisplay)(const DisplayCreateInfo &info);
        void (*DisplayDestroy)(Display* display);
        void* (*GetNativeDisplay)(Display *display);
        void* (*GetNativeWindow)(Display *display);
        void (*PollEvent)();
    };
}
