#pragma once

#include <vector>

namespace gel {
	class GameEntity;

	class GameScene {
	public:
		GameScene() = default;
		virtual ~GameScene() = default;

		void update(double delta_time);

		void addEntity(GameEntity* entity) {
			entities_.push_back(entity);
		}

		void removeEntity(GameEntity* entity) {
			entities_.erase(std::remove(entities_.begin(), entities_.end(), entity), entities_.end());
		}

		const std::vector<GameEntity*>& getEntities() const {
			return entities_;
		}

		int getEntitySize() const {
			return static_cast<int>(entities_.size());
		}

	private:
		std::vector<GameEntity*> entities_;
	};
}