#pragma once




namespace CGEngine {
	class Window;
	
	class FrameBuffer {
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();
		
		
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer& operator=(FrameBuffer&& frameBuffer) noexcept;
		FrameBuffer(FrameBuffer&& frameBuffer) noexcept;

		bool init();

		void bind();
		void unbind();
		unsigned int m_texture_id;
	private:

		unsigned int m_id;
		
		unsigned int rbo;
		unsigned int m_width;
		unsigned int m_height;

	};
}