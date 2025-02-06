#pragma once

#include <glm/glm.hpp>

#include <array>
#include <unordered_map>

namespace hive {
    enum class InputKey {
        // Alphabet keys
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // Number keys
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        // Function keys
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        // Special keys
        Escape,
        Space,
        Enter,
        Backspace,
        Tab,
        CapsLock,
        LeftShift,
        RightShift,
        LeftCtrl,
        RightCtrl,
        LeftAlt,
        RightAlt,
        LeftSuper,  // Windows key, Command key on Mac
        RightSuper, // Windows key, Command key on Mac
        Menu,       // Context menu key
        PrintScreen,
        ScrollLock,
        Pause,

        // Arrow keys
        Up,
        Down,
        Left,
        Right,

        // Numpad keys
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
        Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadAdd,
        NumpadSubtract,
        NumpadMultiply,
        NumpadDivide,
        NumpadDecimal,
        NumpadEnter,

        // Other keys
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,

        // Punctuation and symbols
        Grave,          // ` and ~ Key
        Minus,          // - and _ Key
        Equal,          // = and + Key
        LeftBracket,    // [ and { Key
        RightBracket,   // ] and } Key
        Backslash,      // \ and | Key
        Semicolon,      // ; and : Key
        Apostrophe,     // ' and " Key 
        Comma,          // , and < Key
        Period,         // . and > Key
        Slash,          // / and ? Key

        // Unknown key
        Unknown
    };

	enum class MouseButton {
        Button1 = 1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
		Last = Button8,
        Left = Button1,
		Right = Button2,
		Middle = Button3
	};

    enum class InputState{
        None,
        Pressed,
        Held,
        Released
    };

    class Window;

    class Input {
        // For now, Support a single input device 
	public:
        explicit Input() = default;
		[[nodiscard]] static InputState getKeyState(InputKey key);
		[[nodiscard]] static bool isKeyPressed(InputKey key);
        [[nodiscard]] static InputState getMouseKeyState(MouseButton button);
		[[nodiscard]] static bool isMouseKeyPressed(MouseButton button);
		[[nodiscard]] static glm::vec2 getMousePosition();
		[[nodiscard]] static glm::vec2 getMouseScroll();
        void subscribeToWindowEvents(Window& window);
        void on_update(f32 dt);
    private:
		void keyCallback(InputKey inputKey, InputState state);
		void mouseCallback(f32 xpos, f32 ypos);
		void mouseKeyCallback(MouseButton button, InputState state);
		void mouseScrollCallback(f32 xoffset, f32 yoffset);

		static std::unordered_map<InputKey, InputState> key_states_;
		static std::array<InputState, static_cast<u32>(MouseButton::Last)> mouse_key_states_;
		static glm::vec2 mouse_pos_;
        static glm::vec2 mouse_scroll_offsets_;
    };
}
