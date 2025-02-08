#include "core/Input.h"

#include "core/Logger.h"
#include "core/Window.h"

#include <functional>
#include <unordered_map>

std::unordered_map<hive::InputKey, hive::InputState> hive::Input::key_states_ = {
    // Alphabet keys
    {hive::InputKey::A, hive::InputState::None},
    {hive::InputKey::B, hive::InputState::None},
    {hive::InputKey::C, hive::InputState::None},
    {hive::InputKey::D, hive::InputState::None},
    {hive::InputKey::E, hive::InputState::None},
    {hive::InputKey::F, hive::InputState::None},
    {hive::InputKey::G, hive::InputState::None},
    {hive::InputKey::H, hive::InputState::None},
    {hive::InputKey::I, hive::InputState::None},
    {hive::InputKey::J, hive::InputState::None},
    {hive::InputKey::K, hive::InputState::None},
    {hive::InputKey::L, hive::InputState::None},
    {hive::InputKey::M, hive::InputState::None},
    {hive::InputKey::N, hive::InputState::None},
    {hive::InputKey::O, hive::InputState::None},
    {hive::InputKey::P, hive::InputState::None},
    {hive::InputKey::Q, hive::InputState::None},
    {hive::InputKey::R, hive::InputState::None},
    {hive::InputKey::S, hive::InputState::None},
    {hive::InputKey::T, hive::InputState::None},
    {hive::InputKey::U, hive::InputState::None},
    {hive::InputKey::V, hive::InputState::None},
    {hive::InputKey::W, hive::InputState::None},
    {hive::InputKey::X, hive::InputState::None},
    {hive::InputKey::Y, hive::InputState::None},
    {hive::InputKey::Z, hive::InputState::None},

    // Number keys
    {hive::InputKey::Num0, hive::InputState::None},
    {hive::InputKey::Num1, hive::InputState::None},
    {hive::InputKey::Num2, hive::InputState::None},
    {hive::InputKey::Num3, hive::InputState::None},
    {hive::InputKey::Num4, hive::InputState::None},
    {hive::InputKey::Num5, hive::InputState::None},
    {hive::InputKey::Num6, hive::InputState::None},
    {hive::InputKey::Num7, hive::InputState::None},
    {hive::InputKey::Num8, hive::InputState::None},
    {hive::InputKey::Num9, hive::InputState::None},

    // Function keys
    {hive::InputKey::F1, hive::InputState::None},
    {hive::InputKey::F2, hive::InputState::None},
    {hive::InputKey::F3, hive::InputState::None},
    {hive::InputKey::F4, hive::InputState::None},
    {hive::InputKey::F5, hive::InputState::None},
    {hive::InputKey::F6, hive::InputState::None},
    {hive::InputKey::F7, hive::InputState::None},
    {hive::InputKey::F8, hive::InputState::None},
    {hive::InputKey::F9, hive::InputState::None},
    {hive::InputKey::F10, hive::InputState::None},
    {hive::InputKey::F11, hive::InputState::None},
    {hive::InputKey::F12, hive::InputState::None},

    // Special keys
    {hive::InputKey::Escape, hive::InputState::None},
    {hive::InputKey::Space, hive::InputState::None},
    {hive::InputKey::Enter, hive::InputState::None},
    {hive::InputKey::Backspace, hive::InputState::None},
    {hive::InputKey::Tab, hive::InputState::None},
    {hive::InputKey::CapsLock, hive::InputState::None},
    {hive::InputKey::LeftShift, hive::InputState::None},
    {hive::InputKey::RightShift, hive::InputState::None},
    {hive::InputKey::LeftCtrl, hive::InputState::None},
    {hive::InputKey::RightCtrl, hive::InputState::None},
    {hive::InputKey::LeftAlt, hive::InputState::None},
    {hive::InputKey::RightAlt, hive::InputState::None},
    {hive::InputKey::LeftSuper, hive::InputState::None},  // Windows key, Command key on Mac
    {hive::InputKey::RightSuper, hive::InputState::None}, // Windows key, Command key on Mac
    {hive::InputKey::Menu, hive::InputState::None},       // Context menu key
    {hive::InputKey::PrintScreen, hive::InputState::None},
    {hive::InputKey::ScrollLock, hive::InputState::None},
    {hive::InputKey::Pause, hive::InputState::None},

    // Arrow keys
    {hive::InputKey::Up, hive::InputState::None},
    {hive::InputKey::Down, hive::InputState::None},
    {hive::InputKey::Left, hive::InputState::None},
    {hive::InputKey::Right, hive::InputState::None},

    // Numpad keys
    {hive::InputKey::Numpad0, hive::InputState::None},
    {hive::InputKey::Numpad1, hive::InputState::None},
    {hive::InputKey::Numpad2, hive::InputState::None},
    {hive::InputKey::Numpad3, hive::InputState::None},
    {hive::InputKey::Numpad4, hive::InputState::None},
    {hive::InputKey::Numpad5, hive::InputState::None},
    {hive::InputKey::Numpad6, hive::InputState::None},
    {hive::InputKey::Numpad7, hive::InputState::None},
    {hive::InputKey::Numpad8, hive::InputState::None},
    {hive::InputKey::Numpad9, hive::InputState::None},
    {hive::InputKey::NumpadAdd, hive::InputState::None},
    {hive::InputKey::NumpadSubtract, hive::InputState::None},
    {hive::InputKey::NumpadMultiply, hive::InputState::None},
    {hive::InputKey::NumpadDivide, hive::InputState::None},
    {hive::InputKey::NumpadDecimal, hive::InputState::None},
    {hive::InputKey::NumpadEnter, hive::InputState::None},

    // Other keys
    {hive::InputKey::Insert, hive::InputState::None},
    {hive::InputKey::Delete, hive::InputState::None},
    {hive::InputKey::Home, hive::InputState::None},
    {hive::InputKey::End, hive::InputState::None},
    {hive::InputKey::PageUp, hive::InputState::None},
    {hive::InputKey::PageDown, hive::InputState::None},

    // Punctuation and symbols
    {hive::InputKey::Grave, hive::InputState::None},      // ` and ~
    {hive::InputKey::Minus, hive::InputState::None},      // - and _
    {hive::InputKey::Equal, hive::InputState::None},      // = and +
    {hive::InputKey::LeftBracket, hive::InputState::None},// [ and {
    {hive::InputKey::RightBracket, hive::InputState::None},// ] and }
    {hive::InputKey::Backslash, hive::InputState::None},  // \ and |
    {hive::InputKey::Semicolon, hive::InputState::None},  // ; and :
    {hive::InputKey::Apostrophe, hive::InputState::None}, // ' and "
    {hive::InputKey::Comma, hive::InputState::None},      // , and <
    {hive::InputKey::Period, hive::InputState::None},     // . and >
    {hive::InputKey::Slash, hive::InputState::None},      // / and ?

    // Unknown key
    {hive::InputKey::Unknown, hive::InputState::None}
};

std::array<hive::InputState, static_cast<u32>(hive::MouseButton::Last)> hive::Input::mouse_key_states_ = {
	hive::InputState::None,
    hive::InputState::None,
    hive::InputState::None,
    hive::InputState::None,
    hive::InputState::None,
    hive::InputState::None,
    hive::InputState::None,
    hive::InputState::None
};

glm::vec2 hive::Input::mouse_pos_ = glm::vec2(0.0f);
glm::vec2 hive::Input::mouse_scroll_offsets_ = glm::vec2(0.0f);

hive::InputState hive::Input::getKeyState(InputKey key)
{
	auto it = key_states_.find(key);
	if (it != key_states_.end())
	{
		return it->second;
	}
	LOG_WARN("Unknown key: %i", key);
	return InputState::None;
}

bool hive::Input::isKeyPressed(InputKey key)
{
	const auto state = key_states_.at(key);
	return state == InputState::Pressed || state == InputState::Held;
}

hive::InputState hive::Input::getMouseKeyState(MouseButton button)
{
	return mouse_key_states_.at(static_cast<u32>(button));
}

bool hive::Input::isMouseKeyPressed(MouseButton button)
{
	const auto state = mouse_key_states_.at(static_cast<u32>(button));
	return state == InputState::Pressed || state == InputState::Held;
}

glm::vec2 hive::Input::getMousePosition()
{
    return mouse_pos_;
}

glm::vec2 hive::Input::getMouseScroll()
{
    return mouse_scroll_offsets_;
}

void hive::Input::subscribeToWindowEvents(Window& window)
{
	window.subscribeToKeyInputs(std::bind(&Input::keyCallback, this, std::placeholders::_1, std::placeholders::_2));
	window.subscribeToMouseInputs(std::bind(&Input::mouseCallback, this, std::placeholders::_1, std::placeholders::_2));
	window.subscribeToMouseKeyInputs(std::bind(&Input::mouseKeyCallback, this, std::placeholders::_1, std::placeholders::_2));
    window.subscribeToMouseScrollInputs(std::bind(&Input::mouseScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
}

void hive::Input::on_update(f32 dt)
{
	// Reset scroll offsets as they are only relevant for the current frame
	mouse_scroll_offsets_ = glm::vec2(0.0f);
	// TODO: Update input states of buttons pressed to held
}

void hive::Input::keyCallback(InputKey inputKey, InputState state)
{
	// TODO: Release input events to the rest of the engine
	key_states_[inputKey] = state;
	//LOG_INFO("Key: %i, State: %i", inputKey, state);
}

void hive::Input::mouseCallback(f32 xpos, f32 ypos)
{
    // TODO: Release input events to the rest of the engine
	mouse_pos_ = glm::vec2(xpos, ypos);
	//LOG_INFO("Mouse position: %f, %f", xpos, ypos);
}

void hive::Input::mouseKeyCallback(MouseButton button, InputState state)
{
    // TODO: Release input events to the rest of the engine
	mouse_key_states_[static_cast<u32>(button)] = state;
	//LOG_INFO("Mouse Button: %i, State: %i", button, state);
}

void hive::Input::mouseScrollCallback(f32 xoffset, f32 yoffset)
{
    // TODO: Release input events to the rest of the engine
	mouse_scroll_offsets_ = glm::vec2(xoffset, yoffset);
	//LOG_INFO("Mouse Scroll: %f, %f", xoffset, yoffset);
}

