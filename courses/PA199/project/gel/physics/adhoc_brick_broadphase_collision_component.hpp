#pragma once

#include "gem.hpp"
#include "game_entity.hpp"
#include "game_component.hpp"
#include "renderer/sphere_renderer_component.hpp"
#include "renderer/arc_renderer_component.hpp"
#include "physics/rigidbody_component.hpp"

namespace gel {
	class GameEntity; // Forward declaration
	class SphereRendererComponent; // Forward declaration
	class ArcRendererComponent; // Forward declaration

	class AdhocBrickBroadphaseCollisionComponent : public GameComponent {
	public:
		AdhocBrickBroadphaseCollisionComponent(
			GameEntity* ballEntity,
			std::vector<ArcRendererComponent*> arcRenderers,
			int towerBase = 6,
			int towerStack = 3,
			float towerBaseY = -0.25f
		) :
			ballEntity(ballEntity),
			arcMeshes(arcRenderers),
			towerBase(towerBase),
			towerStack(towerStack),
			towerBaseY(towerBaseY)
		{ 
			ballMesh = dynamic_cast<SphereRendererComponent*>(ballEntity->getComponent<SphereRendererComponent>());
			ballRigidbody = dynamic_cast<RigidbodyComponent*>(ballEntity->getComponent<RigidbodyComponent>());
		}

		void render() override {
			// No rendering needed for this component
		}

		float getPaddleYaw(const gem::Quaternion<float>& q) {
			return std::atan2(2.0f * (q.w() * q.y() + q.x() * q.z()),
				1.0f - 2.0f * (q.y() * q.y() + q.z() * q.z()));
		}

		void update(float delta_time) override {
			gem::Vector<float, 3> ballPosition = ballEntity->getPosition();
			ballPosition[1] = 0.0f;
			float ballRadius = ballMesh->radius();

			for (int i = 0; i < towerBase; i++) {
				ArcRendererComponent* arcMesh = arcMeshes[i];
				gem::Vector<float, 3> brickPosition = arcMesh->getEntity()->getPosition();

				if (arcMesh->getEntity()->isEnabled() == false) continue;

				brickPosition[1] = 0.0f;
				float brickOuterRadius = arcMesh->outerRadius();
				float brickInnerRadius = arcMesh->innerRadius();
				float brickAngle = arcMesh->angle();
				float brickRotation = getPaddleYaw(arcMesh->getEntity()->getOrientation());

				checkCollision(
					ballPosition, ballRadius,
					brickPosition, brickRotation,
					brickOuterRadius, brickInnerRadius, brickAngle,
					i
				);
			}
		}

		void towerSwap(int index) {
			int currIndex = index;

			if (!arcMeshes[currIndex]->breakOnCollision()) return;

			arcMeshes[currIndex]->getEntity()->setEnabled(false);

			while (true) {
				int col = currIndex % towerBase;
				int row = currIndex / towerBase;
				int nextRow = row + 1;
				int nextIndex = (nextRow * towerBase + col) - 1;

				if (nextIndex >= static_cast<int>(arcMeshes.size())) break;

				auto* currEntity = arcMeshes[currIndex]->getEntity();
				auto* nextEntity = arcMeshes[nextIndex]->getEntity();

				gem::Vector<float, 3> tempPosition = currEntity->getPosition();
				currEntity->setPosition(nextEntity->getPosition());
				nextEntity->setPosition(tempPosition);

				std::swap(arcMeshes[currIndex], arcMeshes[nextIndex]);

				currIndex = nextIndex;
			}

			arcMeshes[currIndex]->resetStrength();
		}

		bool checkCollision(
			const gem::Vector<float, 3>& ballPosition, float ballRadius,
			const gem::Vector<float, 3>& brickPosition, float brickRotation,
			float brickOuterRadius, float brickInnerRadius, float brickAngle,
			int brickIndex
		) {
			gem::Polar<float> polar = gem::Polar<float>::fromCartesian(ballPosition - brickPosition);

			if ((polar.r() + ballRadius < brickInnerRadius) ||
				(polar.r() - ballRadius > brickOuterRadius)) {
				//std::cout << "No collision: radial check failed." << std::endl;
				return false;
			}

			float paddleCenter = brickRotation - 0.5f * brickAngle;

			float halfArc = brickAngle * 0.5f;
			float localAngle = polar.theta() + paddleCenter;

			while (localAngle > M_PI) localAngle -= 2 * M_PI;
			while (localAngle < -M_PI) localAngle += 2 * M_PI;

			if (std::abs(localAngle) > halfArc) {
				//std::cout << "No collision: angular check failed." << std::endl;
				return false;
			}

			gem::Vector<float, 3> normal = gem::Vector<float, 3>{
				std::cos(polar.theta()),
				0.0f,
				std::sin(polar.theta())
			}.normalize();

			float overlapOut = (polar.r() + ballRadius) - brickOuterRadius;
			float overlapIn = brickInnerRadius - (polar.r() - ballRadius);

			if (overlapOut > 0.0f) normal = -normal;

			gem::Vector<float, 3> v = ballRigidbody->velocity();
			float vDotN = v.dot(normal);

			if (vDotN > 0.0f) {
				ballRigidbody->applyImpulse(normal * ballRigidbody->mass() * 2.0f * (-ballRigidbody->velocity().dot(normal)));
			}

			towerSwap(brickIndex);

			return std::abs(localAngle) <= halfArc;
		}

	private:
		GameEntity* ballEntity;
		SphereRendererComponent* ballMesh;
		RigidbodyComponent* ballRigidbody;

		std::vector<ArcRendererComponent*> arcMeshes;

		int towerBase = 6;
		int towerStack = 3;
		float towerBaseY = -0.25f;
	};
}