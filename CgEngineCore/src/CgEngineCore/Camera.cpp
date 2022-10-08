#include"CgEngineCore/Camera.hpp"
#include<glm/trigonometric.hpp>
#include<glm/ext/matrix_transform.hpp>

namespace CGEngine {
	Camera::Camera(const glm::vec3& position,
		const glm::vec3& rotation,
		const ProjectionMode projection_mode) :
		m_position(position),
		m_rotation(rotation),
		m_projection_mode(projection_mode)
	{
		update_view_matrix();
		update_projection_matrix();
	}
	Camera::~Camera()
	{
	}
	void Camera::set_position(const glm::vec3& position)
	{
		m_position = position;
		m_update_view_matrix = true;
	}
	void Camera::set_rotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_update_view_matrix = true;
	}
	void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		m_update_view_matrix = true;
	}
	void Camera::set_projection_mode(const ProjectionMode projection_mode)
	{
		m_projection_mode = projection_mode;
	}
	glm::mat4 Camera::get_view_matrix() 
	{
		if (m_update_view_matrix) {
			update_view_matrix();
			m_update_view_matrix = false;
		}
		return m_view_matrix;
	}
	void Camera::move_forward(const float delta)
	{
		m_position += m_forward * delta;
		m_update_view_matrix = true;
	}
	void Camera::move_right(const float delta)
	{
		m_position += m_right * delta;
		m_update_view_matrix = true;
	}
	void Camera::move_up(const float delta)
	{
		m_position += m_up * delta;
		m_update_view_matrix = true;
	}
	void Camera::move_and_rot(const glm::vec3& movement_delta, const glm::vec3& rotation_delta)
	{
		m_position += m_forward * movement_delta.x;
		m_position += m_right	* movement_delta.y;
		m_position += m_up * movement_delta.z;

		m_rotation += rotation_delta;
		m_update_view_matrix = true;
	}
	void Camera::update_view_matrix()
	{
		float roll_in_rad = glm::radians(m_rotation.x);
		float pitch_in_rad = glm::radians(m_rotation.y);
		float yaw_in_rad = glm::radians(m_rotation.z);

		
		glm::mat3 rotate_matrix_x(1, 0, 0, 
			0, cos(roll_in_rad), sin(roll_in_rad), 
			0, -sin(roll_in_rad), cos(roll_in_rad));

		float rotate_in_radians_y = glm::radians(-m_rotation.y);
		glm::mat3 rotate_matrix_y(cos(pitch_in_rad), 0, -sin(pitch_in_rad),
			0, 1, 0, 
			sin(pitch_in_rad), 0, cos(pitch_in_rad));

		float rotate_in_radians_z = glm::radians(-m_rotation.z);
		glm::mat3 rotate_matrix_z(cos(yaw_in_rad), sin(yaw_in_rad), 0, 
			-sin(yaw_in_rad), cos(yaw_in_rad), 0, 
			0, 0, 1);

		const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;

		m_forward = glm::normalize(euler_rotate_matrix * s_world_forward);
		m_right = glm::normalize(euler_rotate_matrix * s_world_right);
		m_up = glm::cross(m_right, m_forward);

		m_view_matrix = glm::lookAt(m_position, m_position + m_forward, m_up);
	}
	void Camera::update_projection_matrix()
	{
		if (m_projection_mode == ProjectionMode::Perspective)
		{
			float r = 0.1f;
			float t = 0.1f;
			float f = 10;
			float n = 0.1f;
			m_projection_matrix = glm::mat4(n / r, 0, 0, 0,
				0, n / t, 0, 0,
				0, 0, (-f - n) / (f - n), -1,
				0, 0, -2 * f * n / (f - n), 0);
		}
		else
		{
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1);
		}

	}
}