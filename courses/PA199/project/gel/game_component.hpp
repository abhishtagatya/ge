#pragma once

#include <vector>
#include "gem.hpp"

namespace gel {
	class GameEntity;

	class GameComponent {
	public:
		GameComponent() = default;

		virtual ~GameComponent() = default;
		virtual void update(float delta_time) = 0;
		virtual void render(gem::Matrix4<float> m, gem::Matrix4<float> v, gem::Matrix4<float> p) = 0;

		void linkEntity(GameEntity* entity) {
			this->entity = entity;
		}

		void unlinkEntity() {
			this->entity = nullptr;
		}

		GameEntity* getEntity() const {
			return entity;
		}
	private:
		GameEntity* entity = nullptr;
	};
}