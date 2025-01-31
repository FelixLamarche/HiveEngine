#pragma once
#include "hvpch.h"
#include "core/Logger.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace hive
{
	// Simple Camera in Right-Handed Coordinate System
	class Camera
	{
	public:
		glm::vec3 position{};
		f32 fov = 0.0f;
		f32 near_plane = 0.0f;
		f32 far_plane = 0.0f;
		
		explicit Camera()
		{
			set_rotation({ 0.0f, 0.0f, 0.0f });
		}
		Camera(f32 fov, f32 near_plane, f32 far_plane)
			: fov(fov), near_plane(near_plane), far_plane(far_plane)
		{
			set_rotation({ 0.0f, 0.0f, 0.0f });
		}
		virtual ~Camera() = default;

		[[nodiscard]] glm::mat4 get_view_matrix() const
		{
			return glm::lookAtRH(position, position + front_, up_);
		}
		[[nodiscard]] glm::mat4 get_projection_matrix(f32 aspect_ratio) const
		{
			return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
		}

		// (0,0,0) is aimed towards the negative z-axis
		[[nodiscard]] glm::vec3 get_rotation() const
		{
			return rotation_;
		}
		void set_rotation(const glm::vec3& rotation)
		{
			// The camera should by default by looking down the negative z-axis
			rotation_ = rotation;
			glm::mat4 rotateMat = glm::mat4(1.0f);
			rotateMat = glm::rotate(rotateMat, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
			rotateMat = glm::rotate(rotateMat, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
			rotateMat = glm::rotate(rotateMat, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll

			// Update front, right and up vectors using the updated rotation
			// Front vector points towards the negative z-axis
			front_ = glm::normalize(glm::vec3(rotateMat * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
			right_ = glm::normalize(glm::cross(front_, world_up_));
			up_ = glm::normalize(glm::cross(right_, front_));
		}

		void look_at(const glm::vec3& target)
		{
			front_ = glm::normalize(target - position);
			right_ = glm::normalize(glm::cross(front_, world_up_));
			up_ = glm::normalize(glm::cross(right_, front_));

			const f32 pitch = glm::degrees(glm::asin(front_.y));
			const f32 yaw = glm::degrees(atan2(front_.x, -front_.z));

			rotation_ = glm::vec3(pitch, yaw, 0.0f);
		}

	protected:
		glm::vec3 rotation_{}; // Euler Angles around the corresponding axis, Order: pitch, yaw, roll
		glm::vec3 front_{};
		glm::vec3 up_{};
		glm::vec3 right_{};

		const glm::vec3 world_up_{ 0.0f, 1.0f, 0.0f };
	};
}