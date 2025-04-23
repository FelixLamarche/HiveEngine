#pragma once

namespace hive
{
    enum SystemEventCode
    {
        /** @brief Shuts the application down on the next frame. */
        EVENT_CODE_APPLICATION_QUIT = 0x01,

        /** @brief Keyboard key pressed.
         * Context usage:
         * u16 key_code = data.data.u16[0];
         * u16 repeat_count = data.data.u16[1];
         */
        EVENT_CODE_KEY_PRESSED = 0x02,

        /** @brief Keyboard key released.
         * Context usage:
         * u16 key_code = data.data.u16[0];
         * u16 repeat_count = data.data.u16[1];
         */
        EVENT_CODE_KEY_RELEASED = 0x03,

        /** @brief Mouse button pressed.
         * Context usage:
         * u16 button = data.data.u16[0];
         * u16 x = data.data.i16[1];
         * u16 y = data.data.i16[2];
         */
        EVENT_CODE_BUTTON_PRESSED = 0x04,

        /** @brief Mouse button released.
         * Context usage:
         * u16 button = data.data.u16[0];
         * u16 x = data.data.i16[1];
         * u16 y = data.data.i16[2];
         */
        EVENT_CODE_BUTTON_RELEASED = 0x05,

        /** @brief Mouse button pressed then released.
         * Context usage:
         * u16 button = data.data.u16[0];
         * u16 x = data.data.i16[1];
         * u16 y = data.data.i16[2];
         */
        EVENT_CODE_BUTTON_CLICKED = 0x06,

        /** @brief Mouse moved.
         * Context usage:
         * u16 x = data.data.i16[0];
         * u16 y = data.data.i16[1];
         */
        EVENT_CODE_MOUSE_MOVED = 0x07,

        /** @brief Mouse moved.
         * Context usage:
         * ui z_delta = data.data.i8[0];
         */
        EVENT_CODE_MOUSE_WHEEL = 0x08,

        /** @brief Resized/resolution of a window changed from the OS.
         * Context usage:
         * u16 width = data.data.u16[0];
         * u16 height = data.data.u16[1];
         * Sender is the window itself.
         */
        EVENT_CODE_WINDOW_RESIZED = 0x09,

        // Change the render mode for debugging purposes.
        /* Context usage:
         * i32 mode = context.data.i32[0];
         */
        EVENT_CODE_SET_RENDER_MODE = 0x0A,

        /** @brief Special-purpose debugging event. Context will vary over time. */
        EVENT_CODE_DEBUG0 = 0x10,
        /** @brief Special-purpose debugging event. Context will vary over time. */
        EVENT_CODE_DEBUG1 = 0x11,
        /** @brief Special-purpose debugging event. Context will vary over time. */
        EVENT_CODE_DEBUG2 = 0x12,
        /** @brief Special-purpose debugging event. Context will vary over time. */
        EVENT_CODE_DEBUG3 = 0x13,
        /** @brief Special-purpose debugging event. Context will vary over time. */
        EVENT_CODE_DEBUG4 = 0x14,

        EVENT_CODE_DEBUG5 = 0x15,
        EVENT_CODE_DEBUG6 = 0x16,
        EVENT_CODE_DEBUG7 = 0x17,
        EVENT_CODE_DEBUG8 = 0x18,
        EVENT_CODE_DEBUG9 = 0x19,
        EVENT_CODE_DEBUG10 = 0x1A,
        EVENT_CODE_DEBUG11 = 0x1B,
        EVENT_CODE_DEBUG12 = 0x1C,
        EVENT_CODE_DEBUG13 = 0x1D,
        EVENT_CODE_DEBUG14 = 0x1E,
        EVENT_CODE_DEBUG15 = 0x1F,

        /** @brief The hovered-over object id, if there is one.
         * Context usage:
         * i32 id = context.data.u32[0]; - will be INVALID ID if nothing is hovered over.
         */
        EVENT_CODE_OBJECT_HOVER_ID_CHANGED = 0x20,

        /**
         * @brief An event fired by the renderer backend to indicate when any render targets
         * associated with the default window resources need to be refreshed (i.e. a window resize)
         */
        EVENT_CODE_DEFAULT_RENDERTARGET_REFRESH_REQUIRED = 0x21,

        /**
         * @brief An event fired by the kvar system when a kvar has been updated.
         * Context usage:
         * kvar_change* change = context.data.custom_data.data;
         */
        EVENT_CODE_KVAR_CHANGED = 0x22,

        /**
         * @brief An event fired when a watched asset file has been written to.
         * Context usage:
         * u32 watch_id = context.data.u32[0];
         * kasset* = sender;
         */
        EVENT_CODE_ASSET_HOT_RELOADED = 0x23,

        /**
         * @brief An event fired when a watched asset file has been removed.
         * Context usage:
         * u32 watch_id = context.data.u32[0];
         * vfs_asset_data* = sender
         */
        EVENT_CODE_ASSET_DELETED_FROM_DISK = 0x24,

        /**
         * @brief An event fired when one of a resource's assets has been hot-reloaded.
         * Context usage:
         * u32 watch_id = context.data.u32[0];
         * The sender should be a pointer to the resource whose asset which was hot-reloaded and processed.
         */
        EVENT_CODE_RESOURCE_HOT_RELOADED = 0x25,

        /**
         * @brief An event fired while a button is being held down and the
         * mouse is moved.
         *
         * Context usage:
         * i16 x = context.data.i16[0]
         * i16 y = context.data.i16[1]
         * u16 button = context.data.u16[2]
         */
        EVENT_CODE_MOUSE_DRAGGED = 0x30,

        /**
         * @brief An event fired when a button is pressed and a mouse movement
         * is done while it is pressed.
         *
         * Context usage:
         * i16 x = context.data.i16[0]
         * i16 y = context.data.i16[1]
         * u16 button = context.data.u16[2]
         */
        EVENT_CODE_MOUSE_DRAG_BEGIN = 0x31,

        /**
         * @brief An event fired when a button is released was previously dragging.
         *
         * Context usage:
         * i16 x = context.data.i16[0]
         * i16 y = context.data.i16[1]
         * u16 button = context.data.u16[2]
         */
        EVENT_CODE_MOUSE_DRAG_END = 0x32,

        /** @brief The maximum event code that can be used internally. */
        MAX_EVENT_CODE = 0xFF
    };

    struct Event
    {
        union
        {
            int64 i64[2];
            uint64 u64[2];
            float64 f64[2];

            int32 i32[4];
            uint32 u32[4];
            float32 f32[4];

            int16 i16[8];
            uint16 u16[8];

            int8 i8[16];
            uint8 u8[16];

            union {
                uint64 size;
                void* data;
            } custom_data;
        } data;

    };

    using PFN_On_Event = std::function<void(uint16 code, const Event &event)>;

    class HIVE_API EventManager
    {
        struct EventData
        {
            Event event;
            uint16 code;
        };

        std::queue<EventData> p_events;
        std::unordered_map<int, std::unordered_map<int, PFN_On_Event>> p_callbacks; //code, callbacks
    public:
        EventManager();
        ~EventManager();


        void EventPush(const Event& event, uint16 code);
        void EventFlush();


        struct CallbackID
        {
            int cat, id;
        };
        CallbackID CallbackSubscribe(uint16 code, const PFN_On_Event& callback);
        void CallbackUnSubscribe(CallbackID id);


    };
}