#include "CgEngineCore/Window.hpp"
#include "CgEngineCore/Log.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace CGEngine {
    static bool s_GLFW_initialized = false;
	
	Window::Window(std::string title, const unsigned int width, const unsigned int height):m_title(std::move(title)),m_width(width),m_height(height)
	{
		int result = init();
	}

	Window::~Window()
	{

		shutdown();
        
    }

	void Window::on_update()
	{
        glClearColor(0.4f, .4f, .71f, 1.0f);
       
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);

        /* Poll for and process events */
        glfwPollEvents();

            
        
	}

	int Window::init()
	{
        

        /* Initialize the library */

        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRIT("GLFW failed to init");
                return -1;
            }
                
        }else
        s_GLFW_initialized = true;

        /* Create a windowed mode window and its OpenGL context */
        m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
        if (!m_pWindow)
        {
            LOG_CRIT("Cant create window");
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRIT("Failed to initialize GLAD");
            return -1;
        }


        

        
        return 0;
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

}