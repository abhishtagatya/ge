#pragma once

#include "game_component.hpp"

namespace gel {
	class TestComponent : public GameComponent {
	public:
		TestComponent() = default;

		void update(float delta_time) override;
		void render(gem::Matrix4<float> m, gem::Matrix4<float> v, gem::Matrix4<float> p) override {};
	};
}