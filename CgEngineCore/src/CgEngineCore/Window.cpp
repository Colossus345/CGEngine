#include "CgEngineCore/Window.hpp"
#include "CgEngineCore/Log.hpp"
#include "CgEngineCore/Modules/UIModule.hpp"
#include "CgEngineCore/Rendering/Render_OpenGl.hpp"


#include<GLFW/glfw3.h>
#include<imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>



namespace CGEngine {
   


    

	
    Window::Window(std::string title, const unsigned int width, const unsigned int height) :m_data({ std::move(title) ,width, height })
	{
		int result = init();

        
	}

	Window::~Window()
	{

		shutdown();
        
    }

	void Window::on_update()
	{
        
        
       
        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);

        /* Poll for and process events */
        glfwPollEvents();

            
        
	}

	int Window::init()
	{
        

        /* Initialize the library */


        glfwSetErrorCallback([](int error_code, const char* description) {
            LOG_ERROR("GLFW ERROR: {0}", description);
            });
        
        if (!glfwInit()) {

            LOG_CRIT("GLFW failed to init");
            return -1;
        }
                       

        /* Create a windowed mode window and its OpenGL context */
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), NULL, NULL);
        if (!m_pWindow)
        {
            LOG_CRIT("Cant create window");
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_pWindow);

        if (!Renderer_OpenGL::init(m_pWindow)) {
            LOG_CRIT("Failed initialize opengl render");
            return-1;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetKeyCallback(m_pWindow, []
        (GLFWwindow* pWindow, int key, int scancode, int action, int modes)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                switch (action)
                {
                case GLFW_PRESS:
                {
                    EventKeyPressed event(static_cast<KeyCode> (key), false);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventKeyReleased event(static_cast<KeyCode> (key));
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    EventKeyPressed event(static_cast<KeyCode> (key), true);
                    data.eventCallbackFn(event);
                    break;
                }
                }
            }
        );

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width,int height) 
            {
               
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
            
                EventWindowResize event(width,height);
                data.eventCallbackFn(event);
                

            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double new_x, double new_y)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventMouseMoved event(new_x, new_y);

                data.eventCallbackFn(event);

            });
        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow) {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;

                data.eventCallbackFn(event);
            });
        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {

                Renderer_OpenGL::set_viewport( width, height,0,0);
            }
        );
        UIModule::on_window_create(m_pWindow);
       
        
        
        
        return 0;
	}

	void Window::shutdown()
	{
        UIModule::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

}