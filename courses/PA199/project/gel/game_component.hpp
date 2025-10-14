#pragma once

#include <vector>

namespace gel {
	class GameEntity;

	class GameComponent {
	public:
		GameComponent() = default;

		virtual ~GameComponent() = default;
		virtual void update(double delta_time) = 0;

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