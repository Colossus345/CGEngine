#pragma once
#include"VertexBuffer.hpp"

namespace CGEngine {
	class IndexBuffer {
	public:
		IndexBuffer(const void* data,const size_t count, const VertexBuffer::EUsage usage= VertexBuffer::EUsage::STATIC);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		
		void bind() const;
		static void unbind();
		size_t get_count() const { return m_count; }

	private:
		unsigned int m_id = 0;
		size_t m_count;
	};
}