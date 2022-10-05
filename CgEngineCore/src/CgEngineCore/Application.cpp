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

		
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				//LOG_INFO("[Mouse moved] new pos {0}x{1}", event.x, event.y);
			});
		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event) {
				//LOG_INFO("[Window resized] new width/height {0}x{1}", event.width, event.height);
			});
		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("Window closed");
				m_bCloseWindow = true;
			});

		m_pWindow->set_event_callback([&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			});



		while (!m_bCloseWindow) {
			m_pWindow->on_update();
			on_update();
		}
		m_pWindow = nullptr;
		return 0;
	}
}