#include "game_scene.hpp"
#include "game_entity.hpp"

namespace gel {

	void GameScene::update(double delta_time) {
		for (auto* entity : entities_) {
			entity->update(delta_time);
		}
	}
}