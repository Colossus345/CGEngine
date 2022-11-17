#include<iostream>
#include<memory>

#include"CgEngineCore/Application.hpp"
#include"CgEngineCore/Input.hpp"


#include<imgui/imgui.h>

class CGEngineEditor : public CGEngine::Application {

    glm::vec2 m_init_mouse_pos = {0.f,0.f};

	virtual void on_update() override {
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_W))
        {
            movement_delta.x += 0.05f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_S))
        {
            movement_delta.x -= 0.05f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_A))
        {
            movement_delta.y -= 0.05f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_D))
        {
            movement_delta.y += 0.05f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_E))
        {
            movement_delta.z += 0.05f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_Q))
        {
            movement_delta.z -= 0.05f;
             
        }

        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_UP))
        {
            rotation_delta.y -= 0.5f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_DOWN))
        {
            rotation_delta.y += 0.5f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_RIGHT))
        {
            rotation_delta.z -= 0.5f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_LEFT))
        {
            rotation_delta.z += 0.5f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_P))
        {
            rotation_delta.x += 0.5f;
             
        }
        if (CGEngine::Input::isKeyPressed(CGEngine::KeyCode::KEY_O))
        {
            rotation_delta.x -= 0.5f;
             
        }

        if (CGEngine::Input::isMouseButtonPressed(CGEngine::MouseButton::MOUSE_BUTTON_RIGHT)) {
            glm::vec2 cur_pos = get_cur_pos();
            if (CGEngine::Input::isMouseButtonPressed(CGEngine::MouseButton::MOUSE_BUTTON_LEFT)) {
               
                camera.move_right((m_init_mouse_pos.x - cur_pos.x) / 100.f);
                camera.move_up((m_init_mouse_pos.y - cur_pos.y) / 100.f);

            }
            else {
               
                rotation_delta.z =( m_init_mouse_pos.x - cur_pos.x)/5.f;
                rotation_delta.y = (m_init_mouse_pos.y - cur_pos.y) / 5.f;
                
            }
            m_init_mouse_pos = cur_pos;
        }
        camera.move_and_rot(movement_delta, rotation_delta);
       
	}
    virtual void on_mouse_button_event(const CGEngine::MouseButton button_code, 
        const double x_pos, 
        const double y_pos, 
        const bool is_pressed) 
    {
        m_init_mouse_pos.x = x_pos;
        m_init_mouse_pos.y = y_pos;
    }
	virtual void on_ui_draw() override {

        camera_position[0] = camera.get_camera_position().x;
        camera_position[1] = camera.get_camera_position().y;
        camera_position[2] = camera.get_camera_position().z;
        camera_rotation[0] = camera.get_camera_rotation().x;
        camera_rotation[1] = camera.get_camera_rotation().y;
        camera_rotation[2] = camera.get_camera_rotation().z;


		ImGui::Begin("Editor");
        if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f)) {
            camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
        }

        if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f)) {
            camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        }

		ImGui::Checkbox("Perspective camera", &perspective_camera);

		ImGui::End();
	}

	
    int frame = 0;
 };

int main(int argc,char** argv) {

	auto myApp = std::make_unique<CGEngineEditor>();
    
	int returnCode = myApp->start(1024, 768, "Myapp", argv[0]);

	return 0;
}