#pragma once

#include"glm/glm.hpp"
struct GLFWwindow;

namespace CGEngine {
	
	class UIModule {
	public:
		static void on_window_create(GLFWwindow* pWindow);
		static void on_window_close();
		static void on_ui_draw_begin();
		static void on_ui_draw_end();

		
		static void on_imgui_render(unsigned int framebuffer_tex_id);

		static void ShowExampleAppDockSpace(bool* p_open);

	private:
		
	};

}