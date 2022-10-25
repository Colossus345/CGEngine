#include "FrameBuffer.hpp"
#include"glad/glad.h"
#include"CgEngineCore/Window.hpp"
#include"CgEngineCore/Log.hpp"
namespace CGEngine {
	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) 
		:m_width(width), m_height(height), m_id(0), m_texture_id(0),rbo(0)
	
	{
	
		glGenFramebuffers(1, &m_id);
		
		
	}
	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
	}
	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& frameBuffer) noexcept
	{
		m_id = frameBuffer.m_id;
		m_texture_id = frameBuffer.m_texture_id;
		rbo = frameBuffer.rbo;
		m_width = frameBuffer.m_width;
		m_height = frameBuffer.m_height;

		frameBuffer.m_id = 0;
		frameBuffer.m_texture_id = 0;
		frameBuffer.rbo = 0;
		frameBuffer.m_width = 0;
		frameBuffer.m_height = 0;
		
		return *this;
	}
	FrameBuffer::FrameBuffer(FrameBuffer&& frameBuffer) noexcept
	{
		m_id = frameBuffer.m_id;
		m_texture_id = frameBuffer.m_texture_id;
		rbo = frameBuffer.rbo;
		m_width = frameBuffer.m_width;
		m_height = frameBuffer.m_height;

		frameBuffer.m_id = 0;
		frameBuffer.m_texture_id = 0;
		frameBuffer.rbo = 0;
		frameBuffer.m_width = 0;
		frameBuffer.m_height = 0;
	}
	bool FrameBuffer::init()
	{
		bind();
		glGenTextures(1, &m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);

		
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 

		
		int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if ( result != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_CRIT("ERROR::FRAMEBUFFER: framebuffer is not complete");
			unbind();
		
		}
			
		
		unbind();
		return result;
	}
	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,m_id);
	}
	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}