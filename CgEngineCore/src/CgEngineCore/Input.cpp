#include"CgEngineCore/Input.hpp"

namespace CGEngine {
	bool Input::m_keyPressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};
	
	bool Input::isKeyPressed(const KeyCode key_code)
	{
		return m_keyPressed[static_cast<size_t>(key_code)];
	}
	void Input::PressKey(const KeyCode key_code)
	{
		m_keyPressed[static_cast<size_t>(key_code)] = true;
	}
	void Input::ReleaseKey(const KeyCode key_code)
	{
		m_keyPressed[static_cast<size_t>(key_code)] = false;
	}
}