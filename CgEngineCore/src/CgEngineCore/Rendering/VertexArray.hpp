#pragma once

#include "VertexBuffer.hpp"

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
	
		void add_buffer(const VertexBuffer& vertexbuffer);
		void bind() const;
		static void unbind();

	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
	};
}
