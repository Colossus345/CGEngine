#pragma once
#include"Keys.hpp"
namespace CGEngine {

	class Input {
	public:
		static bool isKeyPressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);

		static bool isMouseButtonPressed(const MouseButton mouse_button);
		static void PressMouseButton(const MouseButton mouse_button);
		static void ReleaseMouseButton(const MouseButton mouse_button);

	private:
		static bool m_key_pressed[];
		static bool m_mouse_buttons_pressed[];
	};

}