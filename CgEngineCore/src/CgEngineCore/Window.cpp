#include "CgEngineCore/Window.hpp"
#include "CgEngineCore/Log.hpp"
#include "CgEngineCore/Rendering/ShaderProgram.hpp"
#include "CgEngineCore/Rendering/VertexBuffer.hpp"
#include "CgEngineCore/Rendering/VertexArray.hpp"

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

    GLfloat positions_colors[] = {
       0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 1.0f
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
    std::unique_ptr<VertexBuffer> p_points_vbo;
    std::unique_ptr<VertexBuffer> p_colors_vbo;
    std::unique_ptr<VertexArray> p_vao_2buf;

    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<VertexArray> p_vao_1buf;
	
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

        


        ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();


        ImGui::Begin("Background color");
        ImGui::ColorEdit4("Background color", m_background_color);
     

       

        static bool use_2_buffers = true;
        ImGui::Checkbox("2 buffers", &use_2_buffers);

        if (use_2_buffers) {
            p_shader_program->bind();
            p_vao_2buf->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);

        }
        else {
            p_shader_program->bind();
            p_vao_1buf->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);

        }
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
        
        BufferLayout buffer_layout_1vec3{

            ShaderDataType::Float3
        };

        p_vao_2buf = std::make_unique<VertexArray>();
        p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points), buffer_layout_1vec3);
        p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors), buffer_layout_1vec3);
        
       
        p_vao_2buf->add_buffer(*p_points_vbo);
        p_vao_2buf->add_buffer(*p_colors_vbo);
   
        BufferLayout buffer_layout_2vec3{

            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        p_vao_1buf = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_2vec3);

        p_vao_1buf->add_buffer(*p_positions_colors_vbo);
        
        return 0;
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

}