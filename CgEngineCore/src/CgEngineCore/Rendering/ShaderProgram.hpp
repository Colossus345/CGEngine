#pragma once

#include <glm/glm.hpp>

namespace CGEngine {

	class ShaderProgram {
	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		
		void bind() const;
		static void unbind();
		bool isCompiled() const { return m_isCompiled; }
		void setMatrix4(const char* name,const glm::mat4& matrix) const ;
		void setVec3(const char* name, const glm::vec3& matrix) const;
		void setInt(const char* name, const int value)const ;
		unsigned int get_id() const { return m_id; }
	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}