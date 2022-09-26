#include "CgEngineCore/Application.hpp"
#include "CgEngineCore/Log.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<spdlog/spdlog.h>


#include<iostream>


namespace  CGEngine{
	Application::Application()
	{
        LOG_INFO("Welcome to engine with spdlog");
	}
	
	Application::~Application()
	{
	}
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(window_width, window_height, title , NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            LOG_CRIT("Failed to initialize GLAD");
            return -1;
        }


        glClearColor(0.4f, .4f, .71f, 1.0f);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        
            on_update();
        }

        glfwTerminate();
        return 0;
	}
}