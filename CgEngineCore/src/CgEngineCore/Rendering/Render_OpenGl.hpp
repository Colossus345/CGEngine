#pragma once
#include <memory>
#include <vector>
struct GLFWwindow;

namespace CGEngine {
	class VertexArray;
	class Model;
	class Texture2D;
	class ShaderProgram;

	class Renderer_OpenGL
	{
	public:
		static bool init(GLFWwindow* pWindow);

		static void draw(const VertexArray& vertexArray);
		static void draw_model( Model& model, ShaderProgram& shader, double& deltaTime);
		static void draw_with_tex(const VertexArray& vertexArray, unsigned int texhan);
		static void set_clear_color(const float r, const float g, const float b, const float a);
		static void clear(bool depthEnable = false,bool stencilEnable = false);
		static void set_viewport(const unsigned int width, const unsigned int height, 
			const unsigned int left_offset, const unsigned int bottom_offset);

		static const char* get_vendor_str();
		static const char* get_renderer_str();
		static const char* get_version_str();

		static unsigned int current_shader;

	};

	

}
