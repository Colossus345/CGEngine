#pragma once

struct GLFWwindow;

namespace CGEngine {

	class UIModule {
	public:
		static void on_window_create(GLFWwindow* pWindow);
		static void on_window_close();
		static void on_window_update_begin();
		static void on_window_update_draw();
		
	};

}