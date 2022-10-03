#include "CgEngineCore/Window.hpp"
#include "CgEngineCore/Log.hpp"
#include "CgEngineCore/Rendering/ShaderProgram.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>
namespace CGEngine {
    static bool s_GLFW_initialized = false;

    GLfloat points[] = {
        0.0f,0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        -0.5f,-0.5f,0.0f
    };

    GLfloat colors[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.f,0.f,
        0.f,0.f,1.f
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;\n"
        "layout(location = 1) in vec3 vertex_color;\n"
        "out vec3 color;\n"
        "void main() {\n"
        "	color = vertex_color;\n"
        "	gl_Position = vec4(vertex_position, 1.0);\n"
        " }";
    const char* fragment_shader =
        "#version 460\n"
		"in vec3 color;\n"
		"out vec4 frag_color;\n"
		"void main() {\n"
		"	frag_color = vec4(color,1.0); \n"
		" }";

    std::unique_ptr<ShaderProgram> p_shader_program;
    GLuint vao;
	
    Window::Window(std::string title, const unsigned int width, const unsigned int height) :m_data({ std::move(title) ,width, height })
	{
		int result = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow,true);
	}

	Window::~Window()
	{

		shutdown();
        
    }

	void Window::on_update()
	{
        
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
       
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        p_shader_program->bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
       


        ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();


        ImGui::Begin("Background color");
        ImGui::ColorEdit4("Background color", m_background_color);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        


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
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), NULL, NULL);
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

        glfwSetWindowUserPointer(m_pWindow, &m_data);

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

                glViewport(0, 0, width, height);
            }
        );
       
        
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }
        
     

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        
        return 0;
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

}