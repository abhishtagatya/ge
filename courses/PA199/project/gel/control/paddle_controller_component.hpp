#pragma once

#include "gem.hpp"
#include "game_component.hpp"
#include "game_entity.hpp"

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gel {
	class GameEntity;

	class PaddleControllerComponent : public GameComponent {
	public:
		PaddleControllerComponent(float speed = 0.1f) : speed_(speed) {};
		void update(float delta_time) override {};
		void render() override {};
		void handleKeyPressed(int key, int scancode, int action, int mods) override {
			bool pressed = false;
			gem::AxisAngle<float> rotateBy;

			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				switch (key) {
				case GLFW_KEY_A:
				case GLFW_KEY_LEFT:
					rotateBy = gem::AxisAngle<float>{ -speed_, 0.0f, 1.0f, 0.0f };
					pressed = true;
					break;
				case GLFW_KEY_D:
				case GLFW_KEY_RIGHT:
					rotateBy = gem::AxisAngle<float>{ speed_, 0.0f, 1.0f, 0.0f };
					pressed = true;
					break;
				default:
					break;
				}

				if (pressed) {
					getEntity()->rotate(rotateBy);
				}
			}
		};
	private:
		float speed_;
	};
}