#include"Render_OpenGl.hpp"

#include<glad/glad.h>
#include<glfw/glfw3.h>

#include"VertexArray.hpp"
#include"Texture2D.hpp"
#include "CgEngineCore/Rendering/Model.hpp"
#include"CgEngineCore/Log.hpp"
#include "ShaderProgram.hpp"

namespace CGEngine {
	unsigned int  Renderer_OpenGL::current_shader;
	

	bool Renderer_OpenGL::init(GLFWwindow* pWindow)
	{
		glfwMakeContextCurrent(pWindow);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			LOG_CRIT("FAILED TO INIT GLAD");
			return false;
		}
		LOG_INFO("OpenGl context initialized:");
		LOG_INFO("Vendor:{0}",get_vendor_str());
		LOG_INFO("Renderer: {0}", get_renderer_str());
		LOG_INFO("Version:{0}",get_version_str());
		return true;
	}

	void Renderer_OpenGL::draw(const VertexArray& vertexArray)
	{
		vertexArray.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray.get_indicies_count()), GL_UNSIGNED_INT, 0);
	}
	void draw_node(ModelNode& node, ShaderProgram& shader, double& deltaTime) {
		for (int i = 0; i < node.meshes.size(); i++) {
			
			node.meshes[i].Draw(Renderer_OpenGL::current_shader, shader, deltaTime);
		}
		for (int i = 0; i < node.childrenNodes.size(); i++) {
			draw_node(node.childrenNodes[i],shader ,deltaTime );
		}
	}
	void Renderer_OpenGL::draw_model(Model& model,ShaderProgram& shader,double& deltaTime)
	{
		for (int i = 0; i < model.nodes.size(); i++) {
			draw_node(model.nodes[i], shader, deltaTime);
		}
	}


	void Renderer_OpenGL::draw_with_tex(const VertexArray& vertexArray, unsigned int texhan)
	{
		vertexArray.bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray.get_indicies_count()), GL_UNSIGNED_INT, nullptr);

	}

	void Renderer_OpenGL::set_clear_color(const float r, const float g, const float b, const float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer_OpenGL::clear(bool depthEnable, bool stencilEnable )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT * depthEnable  | GL_STENCIL_BUFFER_BIT * stencilEnable);
	}

	void Renderer_OpenGL::set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset)
	{
		glViewport(left_offset, bottom_offset, width, height);
	}

	const char* Renderer_OpenGL::get_vendor_str()
	{
		return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	}

	const char* Renderer_OpenGL::get_renderer_str()
	{
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}

	const char* Renderer_OpenGL::get_version_str()
	{
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}

}