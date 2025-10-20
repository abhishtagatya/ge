#include "game_entity.hpp"
#include "game_component.hpp"
#include <vector>

namespace gel {
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

	gem::Matrix4<float> GameEntity::getLocalTransform() const {
		gem::Matrix4<float> t = gem::Matrix4<float>::translation(position_);
		gem::Matrix4<float> r = \
			gem::Matrix4<float>::rotationZ(orientation_[2]) *
			gem::Matrix4<float>::rotationY(orientation_[1]) *
			gem::Matrix4<float>::rotationX(orientation_[0]);
		gem::Matrix4<float> s = gem::Matrix4<float>::scale(scale_);

		return t * r * s;
	}

	gem::Matrix4<float> GameEntity::getInverseLocalTransform() const {
		return getLocalTransform().inverse();
	}

	gem::Matrix4<float> GameEntity::getWorldTransform() const {
		if (parent_) {
			return parent_->getWorldTransform() * getLocalTransform();
		} else {
			return getLocalTransform();
		}
	}

	gem::Matrix4<float> GameEntity::getInverseWorldTransform() const {
		return getWorldTransform().inverse();
	}

	gem::Vector<float, 3> GameEntity::getRightVector() const {
		gem::Matrix4<float> worldTransform = getWorldTransform();
		return gem::Vector<float, 3>{ worldTransform[0][0], worldTransform[1][0], worldTransform[2][0] }.normalize();
	}

	gem::Vector<float, 3> GameEntity::getUpVector() const {
		gem::Matrix4<float> worldTransform = getWorldTransform();
		return gem::Vector<float, 3>{ worldTransform[0][1], worldTransform[1][1], worldTransform[2][1] }.normalize();
	}

	gem::Vector<float, 3> GameEntity::getForwardVector() const {
		gem::Matrix4<float> worldTransform = getWorldTransform();
		return gem::Vector<float, 3>{ worldTransform[0][2], worldTransform[1][2], worldTransform[2][2] }.normalize();
	}
}