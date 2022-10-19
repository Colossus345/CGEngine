#include "CgEngineCore/Application.hpp"
#include "CgEngineCore/Log.hpp"
#include "CgEngineCore/Window.hpp"
#include "CgEngineCore/Input.hpp"

#include "CgEngineCore/Rendering/ShaderProgram.hpp"
#include "CgEngineCore/Rendering/VertexBuffer.hpp"
#include "CgEngineCore/Rendering/VertexArray.hpp"
#include "CgEngineCore/Rendering/IndexBuffer.hpp"
#include "CgEngineCore/Camera.hpp"
#include "CgEngineCore/Rendering/Render_OpenGl.hpp"
#include "CgEngineCore/Rendering/Texture2D.hpp"

#include "CgEngineCore/Rendering/Model.hpp"
#include "CgEngineCore/Modules/UIModule.hpp"


#include"GLFW/glfw3.h"
#include<iostream>
#include"imgui.h"
#include<glm/mat3x3.hpp>
#include<glm/trigonometric.hpp>

#include<iostream>
#include<fstream>
#include<filesystem>

namespace  CGEngine{
    GLfloat positions_colors[] = {
       0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 1.0f

    };

    GLfloat positions_colors2[] = {
       0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  1.f, 0.f,
        0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 0.f,0.f,
        0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f, 1.f,1.f,
        0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 0.f,1.f
    };

    GLuint indicies[] = {
        0,1,2,3,2,1
    };
    void generate_circle(unsigned char* data,
        const unsigned int width,
        const unsigned int height,
        const unsigned int center_x,
        const unsigned int center_y,
        const unsigned int radius,
        const unsigned char color_r,
        const unsigned char color_g,
        const unsigned char color_b)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
                {
                    data[3 * (x + width * y) + 0] = color_r;
                    data[3 * (x + width * y) + 1] = color_g;
                    data[3 * (x + width * y) + 2] = color_b;
                }
            }
        }
    }

    void generate_smile_texture(unsigned char* data,
        const unsigned int width,
        const unsigned int height)
    {
        // background
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;
            }
        }

        // face
        generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

        // smile
        generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
        generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

        // eyes
        generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
        generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
    }

    void generate_quads_texture(unsigned char* data,
        const unsigned int width,
        const unsigned int height)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2)
                {
                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;
                }
                else
                {
                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;
                }
            }
        }
    }

    const char* vertex_shader =
        R"(#version 440 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
        )";
    const char* fragment_shader =
        R"(#version 440
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
//FragColor = vec4(1.f,0.f,0.f,1.f);
}
        )";

    std::unique_ptr<ShaderProgram> p_shader_program;

    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<VertexArray> p_vao;
    std::unique_ptr<IndexBuffer> p_index_buf;
    std::unique_ptr<Texture2D> p_texture_smile;
    std::unique_ptr<Texture2D> p_texture_quad;

    float scale[3] = { 1.f,1.f,1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f,0.f,0.f };
    float m_background_color[4] = { 0.4f, .4f, .71f, 1.0f };

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
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

        m_event_dispatcher.add_event_listener<EventKeyPressed>(
            [&](EventKeyPressed& event) {
                if (event.key_code <= KeyCode::KEY_Z) {
                    if (event.repeated) {
                        LOG_INFO("[EventKeyPresed] {0},repeated", static_cast<char>(event.key_code));
                    }
                    else {
                        LOG_INFO("[EventKey pressed]:{0}", static_cast<char>(event.key_code));
                    }
               }
                
               
                
                Input::PressKey(event.key_code);
            });

        m_event_dispatcher.add_event_listener<EventKeyReleased>(
            [&](EventKeyReleased& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    LOG_INFO("[Key released: {0}", static_cast<char>(event.key_code));
                }
                Input::ReleaseKey(event.key_code);
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed& event) {
                LOG_INFO("[Mouse button pressed:{0}]", static_cast<int>(event.mouse_button));
                Input::PressMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
                }

            );
        m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased& event)
            {
                LOG_INFO("[Mouse button released:{0}]", static_cast<int>(event.mouse_button));
                Input::ReleaseMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
            });

        m_pWindow->set_event_callback([&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			});


        const unsigned int width = 1000;
        const unsigned int height = 1000;
        const unsigned int channels = 1000;
        auto* data = new unsigned char[width * height*channels];
        
        
        generate_smile_texture(data, width, height);
        p_texture_smile = std::make_unique<Texture2D>(data, width, height);
        p_texture_smile->bind(0);

        generate_quads_texture(data, width, height);
        p_texture_quad = std::make_unique<Texture2D>(data, width, height);
        p_texture_quad->bind(1);

        delete[] data;
        ///////////////////////////////////////////////////////////////////
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }
        stbi_set_flip_vertically_on_load(true);


        Model cube("C:/Users/Syndafloden/Documents/CgEngine/assets/backpack/backpack.obj");
        //Model cube("C:\\Users\\Syndafloden\\Documents\\CgEngine\\assets\\cube.obj");

        BufferLayout buffer_layout_2vec3{

            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>();

        p_positions_colors_vbo->init(positions_colors2, sizeof(positions_colors2), buffer_layout_2vec3);
        
        p_index_buf = std::make_unique<IndexBuffer>();
        p_index_buf->init(indicies, sizeof(indicies) / sizeof(GLuint));
        
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buf);
		///////////////////////////////////////////////////////////////////

        static int current_frame = 0;
        glEnable(GL_DEPTH_TEST);
		while (!m_bCloseWindow) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            //LOG_INFO("FPS:{0}",1 / deltaTime);

            Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
            Renderer_OpenGL::clear();
            /* Render here */

            p_shader_program->bind();

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
                translate[0], translate[1], translate[2], 1);

            glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;

            p_shader_program->setMatrix4("model_matrix", model_matrix);
            
            camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

            p_shader_program->setMatrix4("view", camera.get_view_matrix());
            p_shader_program->setMatrix4("projection", camera.get_projection_matrix());

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // смещаем вниз чтобы быть в центре сцены
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
            p_shader_program->setMatrix4("model", model);

            for (int i = 0; i < cube.meshes.size(); i++) {
                cube.meshes[i].Draw(p_shader_program->get_id());
            }
            //Renderer_OpenGL::draw(*p_vao);


           


           

            //---------------------------------------//
            UIModule::on_ui_draw_begin();
            bool show = true;
            UIModule::ShowExampleAppDockSpace(&show);
            
            ImGui::Begin("Background Color Window");
            ImGui::ColorEdit4("Background Color", m_background_color);
            ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
            ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
            ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
            ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
            ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
            ImGui::Checkbox("Perspective camera", &perspective_camera);
            ImGui::End();
            //---------------------------------------//

            on_ui_draw();

            UIModule::on_ui_draw_end();
			m_pWindow->on_update();
			on_update();
		}
		m_pWindow = nullptr;
		return 0;
	}
    glm::vec2 Application::get_cur_pos() const
    {
        
        return m_pWindow->get_cur_pos();
    }
}