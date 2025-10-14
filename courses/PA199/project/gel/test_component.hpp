#pragma once

#include "game_component.hpp"

namespace gel {
	class TestComponent : public GameComponent {
	public:
		TestComponent() = default;

		void update(double delta_time) override;
	};
}