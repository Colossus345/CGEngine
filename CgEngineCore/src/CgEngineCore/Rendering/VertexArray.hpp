#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include<vector>

namespace CGEngine {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& vertexArray) noexcept;
	
		void add_vertex_buffer(const VertexBuffer& vertexbuffer);
		void set_index_buffer(const IndexBuffer& indexbuffer);
		void bind() const;
		static void unbind();
		size_t get_indicies_count() const { return m_indicies_count; }
	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
		size_t m_indicies_count = 0;
	};
}
