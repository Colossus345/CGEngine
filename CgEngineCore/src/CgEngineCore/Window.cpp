#include "CgEngineCore/Window.hpp"
#include "CgEngineCore/Log.hpp"
#include "CgEngineCore/Rendering/ShaderProgram.hpp"
#include "CgEngineCore/Rendering/VertexBuffer.hpp"
#include "CgEngineCore/Rendering/VertexArray.hpp"
#include "CgEngineCore/Rendering/IndexBuffer.hpp"
#include "CgEngineCore/Camera.hpp"
#include "CgEngineCore/Modules/UIModule.hpp"

#include "CgEngineCore/Rendering/Render_OpenGl.hpp"


#include<GLFW/glfw3.h>


#include<imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>
#include<glm/mat3x3.hpp>
#include<glm/trigonometric.hpp>


namespace CGEngine {
   


    GLfloat positions_colors[] = {
       0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 1.0f

    };

    GLfloat positions_colors2[] = {
       -0.5f,  -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
       0.5f, 0.5f, 0.0f,   1.0f, 0.0f ,0.0f
    };

    GLuint indicies[] = {
        0,1,2,3,2,1
    };

    const char* vertex_shader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_color;
           uniform mat4 model_matrix;
           uniform mat4 view_projection_matrix;
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
           }
        )";
    const char* fragment_shader =
        R"(#version 460
           in vec3 color;
           out vec4 frag_color;
           void main() {
              frag_color = vec4(color, 1.0);
           }
        )";

    std::unique_ptr<ShaderProgram> p_shader_program;

    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<VertexArray> p_vao;
    std::unique_ptr<IndexBuffer> p_index_buf;
    float scale[3] = { 1.f,1.f,1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f,0.f,0.f };

    float camera_position[3] = { 0.f,0.f,1.f };
    float camera_rotation[3] = { 0.f,0.f,0.f };

    bool persperctive_camera = false;

    Camera camera;

	
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
        
        Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        Renderer_OpenGL::clear();
        /* Render here */
        
        
        UIModule::on_window_update_begin();
  

        //ImGui::ShowDemoWindow();


        ImGui::Begin("Background color");
        ImGui::ColorEdit4("Background color", m_background_color);
        ImGui::SliderFloat3("Scale", scale, 0.1f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.0f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);
        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &persperctive_camera);

        glm::mat4 scale_matrix(
            scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1);
        float rotate_in_rad = glm::radians(rotate);
        glm::mat4 rotate_matrix(
            cos(rotate_in_rad), sin(rotate_in_rad), 0, 0,
            -sin(rotate_in_rad), cos(rotate_in_rad), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2],1);

        glm::mat4 model_matrix = translate_matrix*rotate_matrix*scale_matrix;

        p_shader_program->setMatrix4("model_matrix", model_matrix);
        camera.set_position_rotation(glm::vec3(camera_position[0], camera_position[1], camera_position[2])
            , glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        camera.set_projection_mode(persperctive_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

        p_shader_program->setMatrix4("view_projection_matrix", camera.get_projection_matrix()*camera.get_view_matrix());

        p_shader_program->bind();

        Renderer_OpenGL::draw(*p_vao);
        
        

        
        ImGui::End();

        UIModule::on_window_update_draw();
       
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
       
        
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }
        


   
        BufferLayout buffer_layout_2vec3{

            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors2, sizeof(positions_colors2), buffer_layout_2vec3);

        p_index_buf = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(GLuint));
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buf);
        
        return 0;
	}

	void Window::shutdown()
	{
        UIModule::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

}