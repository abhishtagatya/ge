#pragma once

#include <vector>

#include "gem.hpp"


namespace gel {
	class GameComponent;

	class GameEntity {
	public:
		GameEntity() = default;

		GameEntity(const gem::Vector<float, 3>& position,
			const gem::Vector<float, 3>& orientation,
			const gem::Vector<float, 3>& scale)
			: position_(position), orientation_(orientation), scale_(scale) {
		}

		virtual ~GameEntity() = default;

		void addChild(GameEntity* child);
		void removeChild(GameEntity* child);

		void addComponent(GameComponent* comp);
		void removeComponent(GameComponent* comp);

		template<typename T>
		T* getComponent() const {
			for (auto* comp : component_) {
				if (auto* casted = dynamic_cast<T*>(comp)) {
					return casted;
				}
			}
			return nullptr;
		}

		const gem::Vector<float, 3>& getPosition() const { return position_; }
		void setPosition(const gem::Vector<float, 3>& position) { position_ = position; }

		const gem::Vector<float, 3>& getOrientation() const { return orientation_; }
		void setOrientation(const gem::Vector<float, 3>& orientation) { orientation_ = orientation; }

		const gem::Vector<float, 3>& getScale() const { return scale_; }
		void setScale(const gem::Vector<float, 3>& scale) { scale_ = scale; }

		GameEntity* getParent() const { return parent_; }
		const std::vector<GameEntity*>& getChildren() const { return children_; }
		const std::vector<GameComponent*>& getComponents() const { return component_; }

		gem::Matrix4<float> getLocalTransform() const;
		gem::Matrix4<float> getInverseLocalTransform() const;
		gem::Matrix4<float> getWorldTransform() const;
		gem::Matrix4<float> getInverseWorldTransform() const;

		gem::Vector<float, 3> getUpVector() const;
		gem::Vector<float, 3> getRightVector() const;
		gem::Vector<float, 3> getForwardVector() const;

	private:
		gem::Vector<float, 3> position_{ 0.0f, 0.0f, 0.0f };
		gem::Vector<float, 3> orientation_{ 0.0f, 0.0f, 0.0f };
		gem::Vector<float, 3> scale_{ 1.0f, 1.0f, 1.0f };

		GameEntity* parent_ = nullptr;
		std::vector<GameEntity*> children_;

		std::vector<GameComponent*> component_;
	};
}