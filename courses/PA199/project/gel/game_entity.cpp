#include "game_entity.hpp"
#include "game_component.hpp"
#include <vector>

namespace gel {
	void GameEntity::update(double delta_time) {
		for (GameComponent* comp : component_) {
			comp->update(delta_time);
		}

		for (GameEntity* child : children_) {
			child->update(delta_time);
		}
	}

	void GameEntity::addChild(GameEntity* child) {
		children_.push_back(child);
		child->parent_ = this;
	}

	void GameEntity::removeChild(GameEntity* child) {
		children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
		child->parent_ = nullptr;
	}

	void GameEntity::addComponent(GameComponent* comp) {
		component_.push_back(comp);
		comp->linkEntity(this);
	}

	void GameEntity::removeComponent(GameComponent* comp) {
		component_.erase(std::remove(component_.begin(), component_.end(), comp), component_.end());
		comp->unlinkEntity();
	}
}