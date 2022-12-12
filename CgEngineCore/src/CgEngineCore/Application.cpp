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
#include "CgEngineCore/Rendering/FrameBuffer.hpp"

#include "CgEngineCore/Rendering/Model.hpp"
#include "CgEngineCore/Modules/UIModule.hpp"
#include "CgEngineCore/Utils/ResourseManager.hpp"


#include"GLFW/glfw3.h"
#include<iostream>
#include"imgui.h"

#include<glm/mat3x3.hpp>
#include<glm/trigonometric.hpp>

#include<iostream>
#include<fstream>
#include<filesystem>

namespace  CGEngine{

    void test(std::shared_ptr<VertexArray> a) {

    }

    GLfloat positions_colors_coords[] = {
        0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   2.f, -1.f,
        0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  -1.f, -1.f,
        0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   2.f,  2.f,
        0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  -1.f,  2.f
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
   
    



    std::unique_ptr<ShaderProgram> p_screen_shader;
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<VertexArray> p_vao;
    std::unique_ptr<IndexBuffer> p_index_buf;
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
        //stbi_set_flip_vertically_on_load(true);
	}
	
	Application::~Application()
	{
		LOG_INFO("Close app");
	}
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title,char* exepath)
	{
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);


        ResourseManager::getExeDir(std::string(exepath));
        
        
        glEnable(GL_TEXTURE_2D);
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


        
        
       

        
        ///////////////////////////////////////////////////////////////////


        

        auto spider = ResourseManager::loadOBJ("res/hip/hip.dae");
        //auto cat = ResourseManager::loadOBJ("res/cat/cat.obj");
        auto miss = ResourseManager::loadTexture("");
        


        p_framebuffer = std::make_unique<FrameBuffer>(m_pWindow->get_width(), m_pWindow->get_height());
        
        if (!p_framebuffer->init()){}
        static int current_frame = 0;
        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        
       
        ResourseManager::loadTexture("res/miss.png");

        auto missing = ResourseManager::getTexture("res/miss.png");

        auto p_shader_program = ResourseManager::loadShader("default_shader","res/shaders/MainVertextShader.vert", "res/shaders/MainFragmentShader.frag");

        auto p_light_shader = ResourseManager::loadShader("light_shader", "res/shaders/LightVertextShader.vert", "res/shaders/LightFragmentShader.frag");

        auto tex_shader = ResourseManager::loadShader("basic", "res/shaders/BasicTex.vert", "res/shaders/BasicTex.frag");

        while (!m_bCloseWindow) {

            //
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            float fps = 1 / deltaTime;
            std::string title = "FPS:" + std::to_string(fps);
            glfwSetWindowTitle(m_pWindow->m_pWindow, (title).c_str());


            p_framebuffer->bind();
            glEnable(GL_DEPTH_TEST);
            Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
            Renderer_OpenGL::clear(1, 1);


            p_shader_program->bind();

            glm::mat4 scale_matrix(scale[0], 0, 0, 0,
                0, scale[1], 0, 0,
                0, 0, scale[2], 0,
                0, 0, 0, 1);
            float rotate_in_radians = glm::radians(rotate);
            glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
                -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
            glm::mat4 translate_matrix(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                translate[0], translate[1], translate[2], 1);

            glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
            
            
            model_matrix = glm::scale(model_matrix, glm::vec3(0.01f));
            p_shader_program->setMatrix4("model_matrix", model_matrix);

            camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
            Renderer_OpenGL::draw_model(*spider);
            p_shader_program->setMatrix4("view", camera.get_view_matrix());
            p_shader_program->setMatrix4("projection", camera.get_projection_matrix());
            
            
            p_shader_program->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            p_shader_program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            p_shader_program->setVec3("lightPos", glm::vec3(1.2f));
            p_shader_program->setVec3("viewPos", camera.get_camera_position());
            
            
            
            glm::mat4 model = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
            model_matrix = glm::scale(model_matrix, glm::vec3(1.f));
            model_matrix = glm::rotate(model_matrix, glm::radians(90.f),glm::vec3(1,1,1));
            p_shader_program->setMatrix4("model_matrix", model_matrix);


            

            

           
        

           

            p_light_shader->bind();


            p_light_shader->setMatrix4("model_matrix", model_matrix);

            camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
           
            
            p_light_shader->setMatrix4("view", camera.get_view_matrix());
            p_light_shader->setMatrix4("projection", camera.get_projection_matrix());

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 1.0f, 1.0f)); 
            model = glm::scale(model, glm::vec3(0.5f));	
            p_light_shader->setMatrix4("model", model);
            //Renderer_OpenGL::draw_model(*cube);
        
            p_framebuffer->unbind();
                

            

           

            //---------------------------------------//
            UIModule::on_ui_draw_begin();
            bool show = true;
            //UIModule::ShowExampleAppDockSpace(&show);
            

            UIModule::on_imgui_render(p_framebuffer->m_texture_id);

            ImGui::Begin("Background Color Window");
            ImGui::ColorEdit4("Background Color", m_background_color);
            ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
            ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
            ImGui::SliderFloat3("translate", translate, -10.f, 10.f);
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