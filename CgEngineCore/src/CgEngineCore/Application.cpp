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
#include "CgEngineCore/Rendering/StencilBuffer.hpp"
#include "CgEngineCore/Rendering/FrameBuffer.hpp"


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

    float quadVertices[] = { 
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint indicies[] = {
        0,1,2,3,2,1
    };

    const char* vertex_shader =
        R"(#version 440 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model_matrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    
    FragPos = vec3(model_matrix*vec4(aPos,1.0f));
    Normal = mat3(transpose(inverse(model_matrix))) * aNormal;  
    gl_Position = projection * view * model_matrix * vec4(aPos, 1.0);
}
        )";
    const char* lvs =
        R"(#version 440 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
        )";
    const char* fls =
        R"(#version 440
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{    
    FragColor = vec4(1.f);
}
        )";
    const char* fragment_shader =
        R"(#version 440
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  
  
uniform sampler2D texture_diffuse1;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = texture(texture_diffuse1, TexCoords)*vec4(result,1.);
    
}
        )";
    const char* f_framebuffer_shader = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, TexCoords);
})";
    const char* v_framebuffer_shader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";


    const char* f_framebuffer_screenShader =  R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} )";
    const char* v_framebuffer_screenShader =  R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
})";



    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<ShaderProgram> p_light_shader;
    std::unique_ptr<ShaderProgram> p_framebuffer_shader;
    std::unique_ptr<ShaderProgram> p_screen_shader;
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<VertexArray> p_vao;
    std::unique_ptr<IndexBuffer> p_index_buf;
    std::unique_ptr<Texture2D> p_texture_miss;
    std::unique_ptr<Texture2D> p_texture_quad;
    std::unique_ptr<FrameBuffer> p_framebuffer;
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
                    LOG_INFO("[Key released]: {0}", static_cast<char>(event.key_code));
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


        int width;
        int height;
        int format;
        unsigned char* data = stbi_load("C:/Users/Syndafloden/Documents/CGEngine/assets/miss.png", &width, &height, &format, 0);
        
        if (!data) {
            LOG_CRIT("Cant load empty tex");
            return-1;
        }
        

        p_texture_miss = std::make_unique<Texture2D>(data, static_cast<unsigned int>(width), static_cast<unsigned int> (height));
        p_texture_miss->bind(0);

        stbi_image_free(data);

       

        
        ///////////////////////////////////////////////////////////////////
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }
        p_light_shader = std::make_unique<ShaderProgram>(lvs, fls);
        if (!p_shader_program->isCompiled()) {
            return false;
        }
        p_framebuffer_shader = std::make_unique<ShaderProgram>( v_framebuffer_shader, f_framebuffer_shader);
        if (!p_framebuffer_shader->isCompiled()) {
            return false;
        }
        p_screen_shader = std::make_unique<ShaderProgram>(v_framebuffer_screenShader, f_framebuffer_screenShader);
        if (!p_screen_shader->isCompiled()) {
            return false;
        }

        stbi_set_flip_vertically_on_load(true);


        Model cube("C:/Users/Syndafloden/Documents/CgEngine/assets/cube.obj");
        Model outcube("C:/Users/Syndafloden/Documents/CgEngine/assets/cube.obj");
        //Model back("C:/Users/Syndafloden/Documents/CgEngine/assets/backpack/backpack.obj");
        Model lightcube("C:/Users/Syndafloden/Documents/CgEngine/assets/cube.obj");

        BufferLayout buffer_layout_2vec3{

            ShaderDataType::Float4,
            ShaderDataType::Float4
        };
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>();

        p_positions_colors_vbo->init(quadVertices, sizeof(quadVertices), buffer_layout_2vec3);
        
        p_index_buf = std::make_unique<IndexBuffer>();
        p_index_buf->init(indicies, sizeof(indicies) / sizeof(GLuint));
        
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buf);
		


        p_framebuffer = std::make_unique<FrameBuffer>(m_pWindow->get_width(), m_pWindow->get_height());
        
        if (!p_framebuffer->init()){}
        static int current_frame = 0;
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        

       

		while (!m_bCloseWindow) {
            
           
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            //LOG_INFO("FPS:{0}",1 / deltaTime);
            float fps = 1 / deltaTime;
            std::string title = "FPS:" +std::to_string(fps) ;
            glfwSetWindowTitle(m_pWindow->m_pWindow,(title).c_str());


            p_framebuffer->bind();
            glEnable(GL_DEPTH_TEST);
            Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
            Renderer_OpenGL::clear(1,1);
            
           

            

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

            p_shader_program->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            p_shader_program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            p_shader_program->setVec3("lightPos", glm::vec3(1.2f));
            p_shader_program->setVec3("viewPos", camera.get_camera_position());

            p_shader_program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 model = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
            model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));	 
            p_shader_program->setMatrix4("model_matrix", model_matrix);
            
           
            
            for (int i = 0; i < cube.meshes.size(); i++) {
                cube.meshes[i].Draw(p_shader_program->get_id());
            }
            
            

           

            p_light_shader->bind();


            p_light_shader->setMatrix4("model_matrix", model_matrix);

            camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
           
            
            p_light_shader->setMatrix4("view", camera.get_view_matrix());
            p_light_shader->setMatrix4("projection", camera.get_projection_matrix());

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 1.0f, 1.0f)); 
            model = glm::scale(model, glm::vec3(0.5f));	
            p_light_shader->setMatrix4("model", model);
            for (int i = 0; i < cube.meshes.size(); i++) {
                lightcube.meshes[i].Draw(p_light_shader->get_id());
            }
            p_framebuffer->unbind();
            glDisable(GL_DEPTH_TEST);
            Renderer_OpenGL::clear();




            p_screen_shader->bind();
        

            

           

            //---------------------------------------//
            UIModule::on_ui_draw_begin();
            bool show = true;
            UIModule::ShowExampleAppDockSpace(&show);
            
            ImGui::Begin("Background Color Window");
            ImGui::ShowDemoWindow();
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