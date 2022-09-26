#include "CgEngineCore/Application.hpp"
#include "CgEngineCore/Log.hpp"
#include "CgEngineCore/Window.hpp"


#include<iostream>


namespace  CGEngine{
	Application::Application()
	{
        LOG_INFO("Start app");
	}
	
	Application::~Application()
	{
		LOG_INFO("Close app");
	}
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);

		while (true) {
			m_pWindow->on_update();
			on_update();
		}
        
	}
}