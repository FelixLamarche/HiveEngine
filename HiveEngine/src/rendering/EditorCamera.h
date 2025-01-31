#pragma once

#include <hvpch.h>

#include "rendering/Camera.h"

namespace hive
{
	class EditorCamera : public Camera
	{
	public:
		enum class Direction {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

		f32 move_speed = 1.5f;
		f32 rotation_speed = 0.06f;

		explicit EditorCamera()
			: Camera(45.0f, 0.1f, 1000.0f)
		{
		}
		virtual ~EditorCamera() = default;

		void move_direction(Direction direction, float dt);
		void mouse_rotation(float x_offset, float y_offset);
	};
}