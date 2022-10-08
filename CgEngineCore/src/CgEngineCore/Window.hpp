#pragma once

#include"CgEngineCore/Events/Event.hpp"

#include<functional>
#include<string>
#include<glm/ext/vector_float2.hpp>

struct GLFWwindow;

namespace CGEngine {

	class Window
	{
	public:
		
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int width, const unsigned int height);
		 ~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator = (const Window&) = delete;

		void on_update();

		unsigned int get_width() const { return m_data.width; }
		unsigned int get_height() const { return m_data.height; }

		glm::vec2 get_cur_pos() const;

		void set_event_callback(const EventCallbackFn callback) {
			m_data.eventCallbackFn = callback;
		}

	private:

		struct WindowData {

			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shutdown();

		WindowData m_data;

		GLFWwindow* m_pWindow;

		

	};
}