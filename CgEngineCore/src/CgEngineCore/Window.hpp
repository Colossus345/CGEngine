#pragma once

#include<string>

class GLFWwindow;

namespace CGEngine {

	class Window
	{
	public:

		Window(std::string title, const unsigned int width, const unsigned int height);
		 ~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator = (const Window&) = delete;

		void on_update();

		unsigned int get_width() const { return m_width; }
		unsigned int get_height() const { return m_height; }

	private:
		int init();
		void shutdown();

		std::string m_title;
		
		GLFWwindow* m_pWindow;

		unsigned int m_width;
		unsigned int m_height;
	};
}