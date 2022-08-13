#pragma once

#include "oak/core/KeyCodes.h"
#include "oak/core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Oak {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
