#include"IndexBuffer.hpp"

#include"CgEngineCore/Log.hpp"

#include<glad/glad.h>
namespace CGEngine {
	constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage) {

		switch (usage) {
		case VertexBuffer::EUsage::STATIC: return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::STREAM: return GL_STREAM_DRAW;
		}
		LOG_ERROR("Unknown vertexbuffer usage");
		return GL_STREAM_DRAW;
	}

	

	IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage):m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usage_to_GLenum(usage));
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept
	{
		m_id = indexBuffer.m_id;
		m_count = indexBuffer.m_count;
		indexBuffer.m_id = 0;
		indexBuffer.m_count = 0;
		return *this;
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
		:m_id(indexBuffer.m_id),
		m_count(indexBuffer.m_count)
	{
		indexBuffer.m_id = 0;
		indexBuffer.m_count = 0;
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}