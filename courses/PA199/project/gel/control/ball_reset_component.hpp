#pragma once

#include "gem.hpp"
#include "game_entity.hpp"
#include "game_component.hpp"

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gel {
	class BallResetComponent : public GameComponent {
	public:
		BallResetComponent(gem::Vector<float, 3> initial_position) : initial_position_(initial_position) {}

		void update(float delta_time) override {}
		void render() override {}

		void handleKeyPressed(int key, int scancode, int action, int mods) override {
			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				switch (key) {
				case GLFW_KEY_R:
					getEntity()->setPosition(initial_position_);
					break;
				default:
					break;
				}
			}
		}

	private:
		gem::Vector<float, 3> initial_position_{ 0.0f, 0.0f, 0.0f };
	};
}