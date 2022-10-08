#pragma once
#include"Keys.hpp"
namespace CGEngine {

	class Input {
	public:
		static bool isKeyPressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);
	private:
		static bool m_keyPressed[static_cast<size_t>(KeyCode::KEY_LAST)];
	};

}