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

	class AdhocPaddleBroadphaseCollisionComponent : public GameComponent {
	public:
		AdhocPaddleBroadphaseCollisionComponent(
			GameEntity* ballEntity,
			GameEntity* paddleEntityA,
			GameEntity* paddleEntityB
		) :
			ballEntity(ballEntity),
			paddleEntityA(paddleEntityA),
			paddleEntityB(paddleEntityB)
		{
			ballMesh = dynamic_cast<SphereRendererComponent*>(ballEntity->getComponent<SphereRendererComponent>());
			ballRigidbody = dynamic_cast<RigidbodyComponent*>(ballEntity->getComponent<RigidbodyComponent>());
			paddleMeshA = dynamic_cast<ArcRendererComponent*>(paddleEntityA->getComponent<ArcRendererComponent>());
			paddleMeshB = dynamic_cast<ArcRendererComponent*>(paddleEntityB->getComponent<ArcRendererComponent>());
		}

		float getPaddleYaw(const gem::Quaternion<float>& q) {
			return std::atan2(2.0f * (q.w() * q.y() + q.x() * q.z()),
				1.0f - 2.0f * (q.y() * q.y() + q.z() * q.z()));
		}

		void update(float delta_time) override {
			gem::Vector<float, 3> ballPosition = ballEntity->getPosition();
			ballPosition[1] = 0.0f;
			float ballRadius = ballMesh->radius();

			gem::Vector<float, 3> paddleAPosition = paddleEntityA->getPosition();
			paddleAPosition[1] = 0.0f;
			float paddleOuterRadiusA = paddleMeshA->outerRadius();
			float paddleInnerRadiusA = paddleMeshA->innerRadius();
			float paddleAngleA = paddleMeshA->angle();
			float paddleRotationA = getPaddleYaw(paddleEntityA->getOrientation());

			gem::Vector<float, 3> paddleBPosition = paddleEntityB->getPosition();
			paddleBPosition[1] = 0.0f;
			float paddleOuterRadiusB = paddleMeshB->outerRadius();
			float paddleInnerRadiusB = paddleMeshB->innerRadius();
			float paddleAngleB = paddleMeshB->angle();
			float paddleRotationB = getPaddleYaw(paddleEntityB->getOrientation());

			if (checkCollision(ballPosition, ballRadius, paddleAPosition, paddleRotationA, paddleOuterRadiusA, paddleInnerRadiusA, paddleAngleA)) {
				//std::cout << "Collision with Paddle A detected!" << std::endl;
				return;
			}


			if (checkCollision(ballPosition, ballRadius, paddleBPosition, paddleRotationB, paddleOuterRadiusB, paddleInnerRadiusB, paddleAngleB)) {
				//std::cout << "Collision with Paddle B detected!" << std::endl;
				return;
			}
		}

		void render() override {}

		bool checkCollision(
			const gem::Vector<float, 3>& ballPosition, float ballRadius,
			const gem::Vector<float, 3>& paddlePosition, float paddleRotation,
			float paddleOuterRadius, float paddleInnerRadius, float paddleAngle
		) {
			gem::Polar<float> polar = gem::Polar<float>::fromCartesian(ballPosition - paddlePosition);

			if ((polar.r() + ballRadius < paddleInnerRadius) ||
				(polar.r() - ballRadius > paddleOuterRadius)) {
				//std::cout << "No collision: radial check failed." << std::endl;
				return false;
			}

			float paddleCenter = paddleRotation - 0.5f * paddleAngle;

			float halfArc = paddleAngle * 0.5f;
			float localAngle = polar.theta() + paddleCenter;

			while (localAngle > M_PI) localAngle -= 2 * M_PI;
			while (localAngle < -M_PI) localAngle += 2 * M_PI;

			if (std::abs(localAngle) > halfArc) {
				//std::cout << "No collision: angular check failed." << std::endl;
				return false;
			}

			//float eps = 0.0001f;

			//if (localAngle <= -halfArc - eps) {
			//	std::cout << "Collision on left end of the paddle!" << std::endl;
			//}
			//else if (localAngle >= halfArc + eps) {
			//	std::cout << "Collision on right end of the paddle!" << std::endl;
			//}
			//else {
			//	std::cout << "Collision on curved side of the paddle!" << std::endl;
			//}

			gem::Vector<float, 3> normal = gem::Vector<float, 3>{
				std::cos(polar.theta()),
				0.0f,
				std::sin(polar.theta())
			}.normalize();

			gem::Vector<float, 3> v = ballRigidbody->velocity();
			float vDotN = v.dot(normal);

			if (vDotN > 0.0f) {
				ballRigidbody->applyImpulse(normal * ballRigidbody->mass() * 2.0f * (-ballRigidbody->velocity().dot(normal)));

				/*float overlapOut = (polar.r() + ballRadius) - paddleOuterRadius;
				float overlapIn = paddleInnerRadius - (polar.r() - ballRadius);

				if (overlapOut > 0.0f) {
					ballEntity->setPosition(ballEntity->getPosition() - normal * overlapOut);
				}
				else if (overlapIn > 0.0f) {
					ballEntity->setPosition(ballEntity->getPosition() + normal * overlapIn);
				}
				else {
					ballEntity->setPosition(ballEntity->getPosition() + normal * 0.0001f);
				}*/
			}

			return std::abs(localAngle) <= halfArc;
		}

	private:
		GameEntity* ballEntity;
		GameEntity* paddleEntityA;
		GameEntity* paddleEntityB;

		SphereRendererComponent* ballMesh;
		ArcRendererComponent* paddleMeshA;
		ArcRendererComponent* paddleMeshB;

		RigidbodyComponent* ballRigidbody;
	};
}