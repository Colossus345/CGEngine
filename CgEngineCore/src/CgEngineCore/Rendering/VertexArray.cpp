#include "VertexArray.hpp"

#include "CgEngineCore/Log.hpp"

#include<glad/glad.h>

namespace CGEngine{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
	{
		m_id = vertexArray.m_id;
		m_elements_count = vertexArray.m_id;
		vertexArray.m_id = 0;
		vertexArray.m_elements_count = 0;
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
		:m_id(vertexArray.m_id), m_elements_count(vertexArray.m_elements_count)
	{
		vertexArray.m_id = 0;
		vertexArray.m_elements_count = 0;
	}

	void VertexArray::add_vertex_buffer(const VertexBuffer& vertexbuffer)
	{
		bind();
		vertexbuffer.bind();


		for (const BufferElement& current_element : vertexbuffer.get_layout().get_elements()) {
			if (current_element.component_type == 0x1404) {
				glEnableVertexAttribArray(m_elements_count);
				glVertexAttribIPointer(
					m_elements_count,
					static_cast<GLint>(current_element.components_count),
					current_element.component_type,
					static_cast<GLsizei>(vertexbuffer.get_layout().get_stride()),
					reinterpret_cast<const void*>(current_element.offset));
				++m_elements_count;
			}
			else {
				glEnableVertexAttribArray(m_elements_count);
				glVertexAttribPointer(
					m_elements_count,
					static_cast<GLint>(current_element.components_count),
					current_element.component_type,
					GL_FALSE,
					static_cast<GLsizei>(vertexbuffer.get_layout().get_stride()),
					reinterpret_cast<const void*>(current_element.offset));
				++m_elements_count;
			}
			
		}

		
	}

	void VertexArray::set_index_buffer(const IndexBuffer& indexbuffer)
	{
		bind();
		indexbuffer.bind();
		m_indicies_count = static_cast<size_t>(indexbuffer.get_count());
		
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);

	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}


	
	


}

