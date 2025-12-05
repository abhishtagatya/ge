#pragma once

#include "gem.hpp"
#include "game_entity.hpp"
#include "game_component.hpp"

namespace gel {
	class GameEntity; // Forward declaration

	class RigidbodyComponent : public GameComponent {
	public:
		RigidbodyComponent(float mass = 1.0f) :
			mass_(mass),
			velocity_{ 0.0f, 0.0f, 0.0f },
			accumulatedForce_{ 0.0f, 0.0f, 0.0f } {
		}

		void applyForce(const gem::Vector<float, 3>& force) {
			accumulatedForce_ = accumulatedForce_ + force;
		}

		void applyImpulse(const gem::Vector<float, 3>& impulse) {
			velocity_ = velocity_ + impulse / mass_;
		}

		void update(float delta_time) override {
			if (mass_ <= 0.0f) return;

			gem::Vector<float, 3> acceleration = accumulatedForce_ / mass_;
			velocity_ = velocity_ + acceleration * delta_time;

			if (auto entity = getEntity()) {
				gem::Vector<float, 3> pos = getEntity()->getPosition();
				pos = pos + velocity_ * delta_time;
				getEntity()->setPosition(pos);
			}

			accumulatedForce_ = { 0.0f, 0.0f, 0.0f };
		}

		void render() override {}

		float mass() const {
			return mass_;
		}

		const gem::Vector<float, 3>& velocity() const {
			return velocity_;
		}

		void setVelocity(const gem::Vector<float, 3>& velocity) {
			velocity_ = velocity;
		}

	private:
		float mass_;
		gem::Vector<float, 3> velocity_;
		gem::Vector<float, 3> accumulatedForce_;
	};
}