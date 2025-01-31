#include "EditorCamera.h"

#include <algorithm>

void hive::EditorCamera::move_direction(EditorCamera::Direction direction, f32 dt)
{
	if (direction == Direction::FORWARD)
	{
		position += front_ * move_speed * dt;
	}
	else if (direction == Direction::BACKWARD)
	{
		position -= front_ * move_speed * dt;
	}
	else if (direction == Direction::LEFT)
	{
		position -= right_ * move_speed * dt;
	}
	else if (direction == Direction::RIGHT)
	{
		position += right_ * move_speed * dt;
	}
	else if (direction == Direction::UP) // Up and Down always refer to the world up and down, as it is more intuitive
	{
		position += up_ * move_speed * dt;
	}
	else
	{
		position -= up_ * move_speed * dt;
	}
}

void hive::EditorCamera::mouse_rotation(f32 x_offset, f32 y_offset)
{
	const f32 MAX_ANGLE = 89.0f; // To prevent the camera from flipping

	glm::vec3 new_rotation = rotation_speed * (-x_offset * glm::vec3(0.0f, 1.0f, 0.0f) + -y_offset * glm::vec3(1.0f, 0.0f, 0.0f));
	new_rotation += rotation_;
	new_rotation.x = std::clamp(new_rotation.x, -MAX_ANGLE, MAX_ANGLE);
	set_rotation(new_rotation);
}