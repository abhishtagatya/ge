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
		virtual void render() = 0;
		virtual void handleKeyPressed(int key, int scancode, int action, int mods) {}

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