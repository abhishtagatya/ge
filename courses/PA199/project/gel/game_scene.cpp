#include "game_scene.hpp"
#include "game_entity.hpp"

namespace gel {

	void GameScene::recursiveUpdate(GameEntity* entity, float delta_time) {
		for (auto* comp : entity->getComponents()) {
			comp->update(delta_time);
		}

		for (auto* child : entity->getChildren()) {
			recursiveUpdate(child, delta_time);
		}
	}

	void GameScene::recursiveRender(GameEntity* entity) {
		gem::Matrix4<float> model = entity->getWorldTransform();
		gem::Matrix4<float> view = mainCamera_->getViewMatrix();
		gem::Matrix4<float> proj = mainCamera_->getProjectionMatrix();

		for (auto* comp : entity->getComponents()) {
			comp->render(model, view, proj);
		}

		for (auto* child : entity->getChildren()) {
			recursiveRender(child);
		}
	}

	void GameScene::update(float delta_time) {
		for (auto* entity : entities_) {
			recursiveUpdate(entity, delta_time);
		}
	}

	void GameScene::render() {
		if (!mainCamera_) return;

		for (auto* entity : entities_) {
			recursiveRender(entity);
		}
	}
}